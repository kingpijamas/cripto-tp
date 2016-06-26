#!/bin/bash

# ./resource/stegosaurus.jpg

rm laimagen.jpg sample.wav
make clean
make &&
./bin/stegowav -embed -in $1 -p ./resource/a.wav -out sample.wav -steg LSB1 &&
touch laimagen.jpg && ./bin/stegowav -extract -p sample.wav -out laimagen.jpg -steg LSB1
