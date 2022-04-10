#!/bin/bash
set -eu

# Yes this is slightly redundant, but also it works!

if [[ "$@" =~ "clean" ]]
then
	echo "Cleaning..."
	rm -rf ./out
	mkdir -p out
	exit 0
fi

mkdir -p out

if [[ "$@" =~ "debug" ]] || [[ "$@" =~ "valgrind" ]]
then
	echo "Building debug version..."
	g++ -g src/main.cpp -Wall -Wunused -Wuninitialized --std=c++17 -o out/banker
else
	echo "Building..."
	g++ src/main.cpp -Wall -Wunused -Wuninitialized --std=c++17 -o out/banker
fi

if [[ "$@" =~ "valgrind" ]]
then
	echo "Running Valgrind..."
	(cd out && valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=2 --track-fds=yes ./banker)
elif [[ "$@" =~ "run" ]]
then
	echo "Running..."
	(cd out && ./banker)
fi
