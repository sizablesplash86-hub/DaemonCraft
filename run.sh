#!/bin/bash

rm daemoncraft
cd build
cmake --build .
mv daemoncraft ..
cd ..
./daemoncraft
