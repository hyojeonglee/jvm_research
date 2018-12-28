#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <target file>"
	exit 1
fi

scp $1 hjlee@147.46.240.20:~/Desktop/
