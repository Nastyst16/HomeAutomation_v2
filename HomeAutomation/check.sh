#!/bin/bash

cd build
cmake ..
make

cp ./HomeAutomation.uf2 /media/nastyst16/RPI-RP2
rm ./HomeAutomation.uf2

sleep 0.5
cd ..
python3 ./uart_server.py
