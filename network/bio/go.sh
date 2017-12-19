#!/bin/bash

cd `dirname $0`

make clean
make

./s &
spid=$!

sleep 1

for ((i=0;i<10;i++)) ;do
    ./c &
    sleep 1
done

pstack $spid
kill $spid

