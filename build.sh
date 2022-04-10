#!/bin/bash
set -eu

mkdir -p out

echo "Building debug version..."
g++ -g src/main.cpp -Wall -Wunused -Wuninitialized --std=c++17 -o out/banker
