#! /bin/bash
g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv`

