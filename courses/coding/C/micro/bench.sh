#!/usr/bin/env bash

march=`arch`

compute_kernel=accumulate
OPTS="-DDOUBLE_PRECISION"
np_real="float64"

# Comment out to use double precision
# OPTS="-DSINGLE_PRECISION"
# np_real="float32"

if [[ "arm64" == "$march" ]]
then
	CFLAGS="-I. -Ofast -DNDEBUG -march=armv8-a+simd -Dcompute_kernel=$compute_kernel  $OPTS"
else
	CFLAGS="-I. -march=core-avx2 -Ofast -DNDEBUG -Wall -pedantic -Dcompute_kernel=$compute_kernel $OPTS"
fi

set -e

c_sources=`(ls accumulate/*.c)`
cpp_sources=`(ls accumulate/*.cpp)`

rm bin/*
rm obj/*

mkdir -p bin
mkdir -p obj
mkdir -p figures

for src in ${c_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	echo $name
	cc $CFLAGS main.c $src -o bin/$name
done	

for src in ${cpp_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	echo $name
	c++ $CFLAGS -std=c++17 -fno-exceptions -fno-rtti -c $src -o obj/"$name".o
	cc  $CFLAGS main.c obj/"$name".o -o  bin/$name
done	

executables=`(ls bin)`

# a=(10 100 10000 1000000 10000000 100000000 1000000000)
# a=(10 100 10000 1000000 10000000 100000000)
a=(10 100 10000 1000000)

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
		./bin/$e $n "dataset/data_"$n"."$np_real".raw" $repeat >> log.txt
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

./make_plot.py "Throughput [GB/s]" 	TP.csv 	figures/$"compute_kernel"_TP.png
./make_plot.py "TTS [s]" 			TTS.csv figures/$"compute_kernel"_TTS.png
