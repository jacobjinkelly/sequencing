#!/bin/bash

# reformat tests into fasta format

for i in tests/*.in
do
    echo $i
    name=$(echo $i | cut -f 1 -d '.')
    mkdir ${name} &> /dev/null
    echo \>${name} > ${name}/p.fa
    echo $(sed '1!d' $i) >> ${name}/p.fa
    echo \>${name} > ${name}/t.fa
    echo $(sed '2!d' $i) >> ${name}/t.fa
done
