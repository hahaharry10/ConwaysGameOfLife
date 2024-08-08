#!/bin/bash

if [ $# != 1 ]
then
	echo "Usage: ./run.sh [num of procs]"
else
	mpiexec -n $1 ./gameOfLifeMPI
fi
