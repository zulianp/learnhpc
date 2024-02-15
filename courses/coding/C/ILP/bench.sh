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

cc  $CFLAGS main.c acc_Vanilla.c -o PlainC
cc  $CFLAGS main.c acc_DLP.c -o ExDLP
cc  $CFLAGS main.c acc_ILP_DLP.c -o ExCombinedDLPILP
cc  $CFLAGS main.c acc_Implicit_ILP_DLP.c -o ImCombinedILPDLP
cc  $CFLAGS main.c acc_Implicit_ILP.c -o ImILP

c++ $CFLAGS -std=c++17 -fno-exceptions -fno-rtti -c acc_std.cpp 
cc  $CFLAGS main.c acc_std.o -o STLAccumulate

executables=("./PlainC" "./ExCombinedDLPILP" "./ImCombinedILPDLP" "./ImILP" "./STLAccumulate") #"./ExDLP"

# a=(10 100 10000 1000000 10000000 100000000 1000000000)
a=(10 100 10000 1000000 10000000 100000000)
# a=(10 100 10000 1000000)

repeat=100

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

echo "# log.txt" > log.txt

for n in ${a[@]}
do
	echo "# Running size $n" >> log.txt
	for e in ${executables[@]}
	do
		$e $n "dataset/data_"$n"."$np_real".raw" $repeat >> log.txt
	done
done

rm -f TTS.csv
rm -f TP.csv

grep -A5 ${executables[0]} log.txt > temp.txt
header=`grep "size" temp.txt  | awk '{print $2}' | tr '\n' ',' | tr -d ' '` 
echo "Size,$header" | sed 's/.$//' > TTS.csv
echo "Size,$header" | sed 's/.$//' > TP.csv

for e in ${executables[@]}
do
	grep -A5 "$e" log.txt > temp.txt
	cat temp.txt
	
	name=`echo $e | tr -d './' | tr '_' ' '`
	tts=`grep TTS temp.txt | awk '{print $2}' | tr '\n' ',' | tr -d ' '`
	tp=`grep Throughput temp.txt | awk '{print $2}' | tr '\n' ',' | tr -d ' '`
	echo "$name,$tts" | sed 's/.$//' >> TTS.csv
	echo "$name,$tp" | sed 's/.$//' >> TP.csv
done

echo "TTS"
cat TTS.csv
echo "TP"
cat TP.csv

./make_plot.py "Throughput [GB/s]" 	TP.csv 	TP.png
./make_plot.py "TTS [s]" 			TTS.csv TTS.png
