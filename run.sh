#!/bin/sh

# Note that this only works on a successful build and it assumes a coordinate binary named coords.dat in the current directory
# I know, sudo in a bash script is bad practice
sudo ./out/LEDPI ./coords.dat
