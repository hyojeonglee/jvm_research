#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <target file>"
	exit 1
fi

TARGET=$1

/lib/modules/`uname -r`/build/scripts/checkpatch.pl -f $TARGET
