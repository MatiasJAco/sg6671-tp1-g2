#!/bin/sh
g++ -lGL -lGLU -lglut main.cpp globals.cpp bezier_test/bezier.cpp spline_test/spline.cpp -o solids
