#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <target file>"
	exit 1
fi

JBIN_DIR=~/jdk_1.8_source/openjdk-8u40-src-b25-10_feb_2015/openjdk/build/linux-x86_64-normal-server-slowdebug/jdk/bin/
echo $JBIN_DIR

echo "[HJLEE] DROP CACHE..."
./dropcache.sh
echo "[HJLEE] COMPLETE"

echo "[HJLEE] START WORKLOAD..."
time ./exec_openjdk.sh $1
echo "[HJLEE] COMPLETE"
