#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <-gcutil result> <-gc result>"
	exit 1
fi

DIR=./jstatlog/

PIDSTR=`jps | grep Executor`
STRARR=($PIDSTR)
PID=${STRARR[0]}
while [ -z "${STRARR[0]}" ]
do
	echo "[WAITING] PID is empty."
	PIDSTR=`jps | grep Executor`
	STRARR=($PIDSTR)
	PID=${STRARR[0]}
done

echo "[COMPLETE] $PID"

sudo jstat -gcutil -h20 -t $PID 3000 3000 > $DIR$1 \
	& sudo jstat -gc -h20 -t $PID 3000 3000 > $DIR$2
