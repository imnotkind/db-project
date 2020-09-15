#!/bin/bash

mkdir haebin307
cp -r base/ table/ util/ resource/ main.cpp format.sh Makefile Readme.txt haebin307/

cd haebin307
make
rm *.o
cd ..

zip -r Project1_haebin307.zip haebin307/
rm -rf haebin307/
