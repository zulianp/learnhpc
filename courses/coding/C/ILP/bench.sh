#!/usr/bin/env bash

march=`arch`



OPTS="-DDOUBLE_PRECISION"
np_real="float64"

# Comment out to use double precision
# OPTS="-DSINGLE_PRECISION"
# np_real="float32"

if [[ "arm64" == "$march" ]]
then
	CFLAGS="-Ofast -DNDEBUG -march=armv8-a+simd  $OPTS"
	# CFLAGS="-march=armv8-a+simd -Ofast -ffast-math -DNDEBUG $OPTS"
	# -march=armv8-a+simd -ftree-vectorize -ffast-math
	# -march=armv8-a+sve
else
	# CFLAGS="-march=core-avx2 -O2 -g -pg  -DNDEBUG -Wall -pedantic $OPTS"
	CFLAGS="-march=core-avx2 -Ofast -DNDEBUG -Wall -pedantic $OPTS"
fi


set -e
# set -x

rm -f *.exe

function measure_time()
{
	echo "Running" $@

	sys=`uname -s`
	data_cmd=date
	if [[ "Darwin" == "$sys" ]]
	then
		# brew install coreutils
		data_cmd=gdate
	fi

	ts=$($data_cmd +%s%N)
	$@
	echo $((($($data_cmd +%s%N) - $ts)/1000)) "microseconds"


}

cc  $CFLAGS main.c acc_Vanilla.c -o acc_Vanilla.exe
cc  $CFLAGS main.c acc_DLP.c -o acc_DLP.exe
cc  $CFLAGS main.c acc_ILP_DLP.c -o acc_ILP_DLP.exe

c++ $CFLAGS -std=c++17 -fno-exceptions -fno-rtti -c acc_std.cpp 
cc  $CFLAGS main.c acc_std.o -o acc_std.exe

a=(10 100 10000 1000000 10000000 100000000 1000000000)
# a=(10 100 10000 1000000 10000000)

echo "std-c++" > stdcpp.txt
echo "C vanilla" > C_vanilla.txt
echo "C_ILP_DLP" > C_ILP_DLP.txt
echo "C_DLP" > C_DLP.txt

if [[ -d "dataset" ]]
then
	echo "Using available dataset folder!"
else
	mkdir -p dataset


for n in ${a[@]}
do
	python3 -c 'import numpy as np; np.linspace(0, 0.0000001, '$n').astype(np.float64).tofile("dataset/data_'$n'.float64.raw")'
	python3 -c 'import numpy as np; np.linspace(0, 0.0000001, '$n').astype(np.float32).tofile("dataset/data_'$n'.float32.raw")'
done
fi

for n in ${a[@]}
do
	echo "Running size $n"
	measure_time ./acc_Vanilla.exe $n "dataset/data_"$n"."$np_real".raw" >> C_vanilla.txt
	measure_time ./acc_ILP_DLP.exe $n "dataset/data_"$n"."$np_real".raw" >> C_ILP_DLP.txt
	# gprof ./acc_ILP_DLP.exe gmon.out > "analysis_"$n".txt"
	measure_time ./acc_DLP.exe 	   $n "dataset/data_"$n"."$np_real".raw" >> C_DLP.txt
	measure_time ./acc_std.exe 	   $n "dataset/data_"$n"."$np_real".raw" >> stdcpp.txt
done

echo "-----------------------"
echo "timings (microseconds)"
echo "-----------------------"

echo "C++ std lib accumulate"
cat stdcpp.txt 		| grep seconds | awk '{print $1}' | tee e1.txt
echo "C vanilla" 
cat C_vanilla.txt 	| grep seconds | awk '{print $1}' | tee e2.txt
echo "C DLP"
cat C_DLP.txt 	| grep seconds | awk '{print $1}' | tee e3.txt
echo "C ILP/DLP"
cat C_ILP_DLP.txt 	| grep seconds | awk '{print $1}' | tee e4.txt

paste e1.txt e2.txt e3.txt e4.txt > timings.txt

echo "-----------------------"
echo "results"
echo "-----------------------"

echo "C++ std lib accumulate"
cat stdcpp.txt | grep result | tr 'result=' ' '
echo "C vanilla" 
cat C_vanilla.txt | grep result | tr 'result=' ' '
echo "C DLP"
cat C_DLP.txt | grep result | tr 'result=' ' '
echo "C ILP/DLP"
cat C_ILP_DLP.txt | grep result | tr 'result=' ' '


./make_plot.py

