#!/bin/sh

mkdir build
gcc raycaster.c -lSDL2 -lm -o build/raycaster.out
./build/raycaster.out
