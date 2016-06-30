#!/bin/bash

if [[ $# -eq 0 ]]
then
      echo "arg 1 : location of obj_file_list.txt"
      exit
fi


CURDIR=$(pwd)
OBJ_FILE_LIST=$CURDIR/$1
DEST_FOLDER=~/nir_lib/

if [[ ! -d $DEST_FOLDER ]]
then
    mkdir -p $DEST_FOLDER
fi

cd $DEST_FOLDER

unlink *

for i in $(cat $OBJ_FILE_LIST)
do
    ln -s $CURDIR/$i .
done
