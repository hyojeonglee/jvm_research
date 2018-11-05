#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <-gcutil result> <-gc result>"
	exit 1
fi

DIR=./jstatlog/

sudo jstat -gcutil -h20 -t `pidof java` 3000 3000 > $DIR$1 \
	& sudo jstat -gc -h20 -t `pidof java` 3000 3000 > $DIR$2
