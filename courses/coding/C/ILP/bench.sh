#!/usr/bin/env bash


set -e
# set -x

rm -f *.exe

function measure_time()
{
	ts=$(date +%s%N)
	$@
	echo $((($(date +%s%N) - $ts)/1000)) "microseconds"
}

cc  -march=core-avx2 -Ofast -DNDEBUG acc_Vanilla.c -o acc_Vanilla.exe
cc  -march=core-avx2 -Ofast -DNDEBUG acc_ILP_DLP.c -o acc_ILP_DLP.exe
c++ -march=core-avx2 -Ofast -DNDEBUG -std=c++17 acc_std.cpp -o acc_std.exe

# a=(10 100 10000 1000000 10000000 1000000000)
a=(10 100 10000 1000000 1000000)

echo "std-c++" > stdcpp.txt
echo "C vanilla" > C_vanilla.txt
echo "C_ILP_DLP" > C_ILP_DLP.txt

if [[ -d "dataset" ]]
then
	echo "Using available dataset folder!"
else
	mkdir -p dataset


for n in ${a[@]}
do
	python3 -c 'import numpy as np; np.linspace(0, 0.000001, '$n').astype(np.float64).tofile("dataset/data_'$n'.float64.raw")'
done
fi

for n in ${a[@]}
do
	measure_time ./acc_Vanilla.exe $n "dataset/data_"$n".float64.raw" >> C_vanilla.txt
	measure_time ./acc_ILP_DLP.exe $n "dataset/data_"$n".float64.raw" >> C_ILP_DLP.txt
	measure_time ./acc_std.exe 	   $n "dataset/data_"$n".float64.raw" >> stdcpp.txt
done


echo "-----------------------"
echo "timings (microseconds)"
echo "-----------------------"

echo "C++ std lib accumulate"
cat stdcpp.txt | grep micro | awk '{print $1}'
echo "C vanilla" 
cat C_vanilla.txt | grep micro | awk '{print $1}'
echo "C ILP/DLP aware"
cat C_ILP_DLP.txt | grep micro | awk '{print $1}'


echo "-----------------------"
echo "results"
echo "-----------------------"

echo "C++ std lib accumulate"
cat stdcpp.txt | grep result | tr 'result=' ' '
echo "C vanilla" 
cat C_vanilla.txt | grep result | tr 'result=' ' '
echo "C ILP/DLP aware"
cat C_ILP_DLP.txt | grep result | tr 'result=' ' '