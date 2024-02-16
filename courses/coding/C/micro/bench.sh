#!/usr/bin/env bash

march=`arch`

CC=cc
CXX=c++

# a=(1000000 2000000 4000000 6000000 8000000 10000000)
a=(1000000 2000000 4000000 6000000 8000000 10000000)
# a=(10 100 10000 1000000 10000000 100000000 1000000000)
# a=(10 100 10000 1000000 10000000 100000000)
# a=(10 100 10000 1000000)
repeat=20

# compute_kernel=accumulate
# compute_kernel=norm
# main=main_reduce.c

compute_kernel=axpy
main=main_binary_inplace.c


# OPTS="-DDOUBLE_PRECISION"
# np_real="float64"

# Comment out to use double precision
OPTS="-DSINGLE_PRECISION"
np_real="float32"

CFLAGS="-I. -mtune=native -Ofast -DNDEBUG -Wall -pedantic -Dcompute_kernel=$compute_kernel "
if [[ "arm64" == "$march" ]]
then
	# CFLAGS="$CFLAGS -march=armv8-a+simd $OPTS"
	CFLAGS="$CFLAGS -march=armv8-a+simd $OPTS"
else
	CFLAGS="$CFLAGS -march=core-avx2 $OPTS"
fi



c_sources=`(ls "$compute_kernel"/*.c)`
cpp_sources=`(ls "$compute_kernel"/*.cpp)`
neon_sources=`(ls "$compute_kernel"/neon/*.c)`

set -e

rm -rf ./bin
rm -rf ./obj
rm -rf ./assembly

mkdir -p ./bin
mkdir -p ./obj
mkdir -p ./figures
mkdir -p ./assembly

for src in ${c_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	$CC $CFLAGS $main $src -o bin/$name
	$CC $CFLAGS -S $src -o assembly/"$name".s
done	

for src in ${cpp_sources[@]}
do
	fullname=`basename $src`
	name="${fullname%.*}"
	$CXX $CFLAGS -std=c++17 -fno-exceptions -fno-rtti -c $src -o obj/"$name".o
	$CC  $CFLAGS $main obj/"$name".o -o  bin/$name
done	

if [[ "arm64" == "$march" ]]
then
	for src in ${neon_sources[@]}
	do
		fullname=`basename $src`
		name="${fullname%.*}"
		$CC $CFLAGS $main $src -o bin/$name
		$CC $CFLAGS -S $src -o assembly/"$name".s
	done	
fi

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

./make_plot.py "Throughput [GB/s]" 	TP.csv 	figures/"$compute_kernel"_TP.pdf
./make_plot.py "TTS [ms]" 			TTS.csv figures/"$compute_kernel"_TTS.pdf
