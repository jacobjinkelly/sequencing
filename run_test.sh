#!/bin/bash

make

if [ $? -ne 0 ]; then
    exit 1
fi

# correctness tests
for i in tests/*/
do
    echo $i
    ./align.exe ${i}p.fa ${i}t.fa
done

# performance tests
echo p.fa chr1.fa
./align.exe p.fa chr1.fa
echo p.fa chr20.fa
./align.exe p.fa chr20.fa

make clean
