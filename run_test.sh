#!/bin/bash

make

for i in tests/*.in
do
    ./align.exe $(<$i)
done

make clean
