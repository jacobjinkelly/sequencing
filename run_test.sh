#!/bin/bash

make

if [ $? -ne 0 ]; then
    exit 1
fi

for i in tests/*/
do
    echo $i
    ./align.exe ${i}p.fasta ${i}t.fasta
done

make clean
