#! /bin/bash

git submodule update --init Dynamixel-Com
git submodule update --init TeensyStep

pio init --ide clion --board teensy35