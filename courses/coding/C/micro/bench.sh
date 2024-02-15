#!/usr/bin/env bash

march=`arch`

CC=cc
CXX=c++

# a=(10 100 10000 1000000 10000000 100000000 1000000000)
# a=(10 100 10000 1000000 10000000 100000000)
a=(10 100 10000 1000000)
repeat=100

compute_kernel=accumulate
OPTS="-DDOUBLE_PRECISION"
np_real="float64"

# Comment out to use double precision
# OPTS="-DSINGLE_PRECISION"
# np_real="float32"

CFLAGS="-I. -Ofast -DNDEBUG -Wall -pedantic -Dcompute_kernel=$compute_kernel "
if [[ "arm64" == "$march" ]]
then
	CFLAGS="$CFLAGS -march=armv8-a+simd $OPTS"
else
	CFLAGS="$CFLAGS -march=core-avx2 $OPTS"
fi

set -e

c_sources=`(ls accumulate/*.c)`
cpp_sources=`(ls accumulate/*.cpp)`

rm ./bin/*
rm ./obj/*

mkdir -p ./bin
mkdir -p ./obj
mkdir -p ./figures

for src in ${c_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	$CC $CFLAGS main.c $src -o bin/$name
done	

for src in ${cpp_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	$CXX $CFLAGS -std=c++17 -fno-exceptions -fno-rtti -c $src -o obj/"$name".o
	$CC  $CFLAGS main.c obj/"$name".o -o  bin/$name
done	

executables=`(ls bin)`

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

any_exec=""
for n in ${a[@]}
do
	echo "# Running size $n" >> log.txt
	for e in ${executables[@]}
	do
		any_exec=$e
		./bin/$e $n "dataset/data_"$n"."$np_real".raw" $repeat >> log.txt
	done
done

rm -f TTS.csv
rm -f TP.csv

grep -A5 $any_exec log.txt > temp.txt
header=`grep "size" temp.txt  | awk '{print $2}' | tr '\n' ',' | tr -d ' '` 
echo "Size,$header" | sed 's/.$//' > TTS.csv
echo "Size,$header" | sed 's/.$//' > TP.csv

for e in ${executables[@]}
do
	grep -A5 "$e" log.txt > temp.txt	
	name=`echo $e | tr -d './' | tr '_' ' '`
	tts=`grep TTS temp.txt | awk '{print $2}' | tr '\n' ',' | tr -d ' '`
	tp=`grep Throughput temp.txt | awk '{print $2}' | tr '\n' ',' | tr -d ' '`
	echo "$name,$tts" | sed 's/.$//' >> TTS.csv
	echo "$name,$tp" | sed 's/.$//' >> TP.csv
done

./make_plot.py "Throughput [GB/s]" 	TP.csv 	figures/$"compute_kernel"_TP.png
./make_plot.py "TTS [s]" 			TTS.csv figures/$"compute_kernel"_TTS.png
