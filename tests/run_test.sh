#!/bin/bash

for i in *.in
do
    ../../align.exe $(<$i)
done
