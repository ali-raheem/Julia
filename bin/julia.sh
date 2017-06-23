#!/bin/sh

SCALEX=1.0
SCALEY=1.0
WIDTH=1000
HEIGHT=1000
ITERATIONS=200
THRESH=1000
C="-0.8 + 0.156 * I"
NUM_THREADS=24

# Replace settings with sed
echo "Configuring..."
sed -e "s/SCALEX/$SCALEX/g;s/SCALEY/$SCALEY/g;s/WIDTH/$WIDTH/g;s/HEIGHT/$HEIGHT/g;s/ITERATIONS/$ITERATIONS/g;s/THRESH/$THRESH/g;s/C/$C/g;s/NUM_THREADS/$NUM_THREADS/g;" ./src/julia.template > julia.tmp.c

# Compile
echo "Compiling..."
gcc -O3 -o julia.tmp julia.tmp.c -lm -lpthread

# Run
echo "Running..."
time ./julia.tmp

# Convert to image
echo "Converting to image..."
./bin/jd2png.py

# Clean up
rm julia.tmp.c
