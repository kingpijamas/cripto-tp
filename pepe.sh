#!/bin/bash

# ./resource/stegosaurus.jpg

rm output.txt sample.wav
make clean
make &&
./bin/stegowav -embed -in $1 -p ./resource/a.wav -out sample.wav -steg LSBE &&
./bin/stegowav -extract -p sample.wav -out output -steg LSBE && cat output.txt