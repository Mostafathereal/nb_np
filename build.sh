#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
cmake --build .

if [ $? -eq 0 ]; then
    echo -e "\n--- Running Program ---\n"
    ./main
else
    echo -e "\nBuild Failed!"
fi