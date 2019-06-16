#!/bin/bash

make

if [ $? -ne 0 ]; then
    exit 1
fi

for i in tests/*.in
do
    echo $i
    ./align.exe $(<$i)
done

make clean
