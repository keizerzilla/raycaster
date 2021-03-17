#!/bin/sh

mkdir build
gcc raycaster.c -lSDL2 -o build/raycaster.out
./build/raycaster.out
