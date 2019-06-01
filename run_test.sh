#!/bin/bash

make

for i in tests/*.in
do
    echo $i
    ./align.exe $(<$i)
done

make clean
