#!/bin/bash

BINDIR=`dirname $0`
cd $BINDIR

TESTDIR=for_test
REPO=dapa

if [ -d $TESTDIR ]
then
	echo "[FAIL] Test directory ($TESTDIR) already exists"
	exit 0
fi

mkdir $TESTDIR
cd $TESTDIR

git clone ../../$REPO
cd $REPO
make

if [ $? -ne 0 ]
then
	echo "[FAIL]"
	cd ../../
	rm -fr $TESTDIR
	exit 0
fi

echo "[SUCCESS]"
cd ../../
rm -fr $TESTDIR
