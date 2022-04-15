#!/bin/bash
set -eu

# Writes a colorful indented message. Very cool!
function sayThing {
	printf "\t\033[32m%s\033[0m\n" "$1"
}

# Yes this is slightly redundant, but also it works!

if [[ "$@" =~ "clean" ]]
then
	sayThing "Cleaning..."
	rm -rf ./out
	mkdir -p out
	exit 0
fi

mkdir -p out

if [[ "$@" =~ "debug" ]] || [[ "$@" =~ "valgrind" ]]
then
	sayThing "Building debug version..."
	g++ -g src/main.cpp -Wall -Wunused -Wuninitialized --std=c++17 -o out/banker
else
	sayThing "Building..."
	g++ src/main.cpp -Wall -Wunused -Wuninitialized --std=c++17 -o out/banker
fi

if [[ "$@" =~ "valgrind" ]]
then
	sayThing "Running Valgrind with example data 1..."
	(cd out && valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=2 --track-fds=yes ./banker ../data1.txt)
elif [[ "$@" =~ "run" ]]
then
	sayThing "Running with all example data..."
	sayThing "	data1:"
	(cd out && ./banker ../data1.txt)
	sayThing "	data2:"
	(cd out && ./banker ../data2.txt)
	sayThing "	data3:"
	(cd out && ./banker ../data3.txt)
fi
