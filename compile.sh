#!/bin/bash

sudo fuser -k -9 5768/tcp
sudo fuser -k -9 5768/tcp
sudo fuser -k -9 25569/tcp
sudo fuser -k -9 25569/tcp
rm daemoncraft
cd build
cmake --build .
mv daemoncraft ..
cd ..
./daemoncraft