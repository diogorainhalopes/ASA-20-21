#!/bin/bash


V=${1}
P=${2}


./randomDAG $V $P > testfile
time ./p1 < testfile



