#!/bin/bash

DEFAULT_INSTALL_DIR=/usr/local/bin
DEP_FILE=./dependencies


# get first conarg
if [ -z "$1" ] ; then
    echo "error: no executable file"    
    exit
else
    EXEC_FILE=$1
fi

# get second conarg
if [ -z "$2" ] ; then
    echo "warning: no install directory given (default '$DEFAULT_INSTALL_DIR')"
    INSTALL_DIR=$DEFAULT_INSTALL_DIR
else
    INSTALL_DIR=$2
fi

# get dependency list
./get-deps $EXEC_FILE > $DEP_FILE

# find and copy dependencies
while read p; do
    if ! cp `where $p` $INSTALL_DIR/
    then
        echo "warning: $p: could not copy file to $INSTALL_DIR/"
    fi    
done < $DEP_FILE



