#!/bin/bash

PID=`pidof python`

echo $PID
echo 'PROCESSING'

if [ $PID -ne 27313 ]
then
	echo 'FINISH'
fi
