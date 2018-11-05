#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <target file>"
	exit 1
fi

java -Xmx60g -XX:ParallelGCThreads=1 -XX:ConcGCThreads=1 $1 
