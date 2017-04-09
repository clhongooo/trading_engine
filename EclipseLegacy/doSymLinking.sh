#!/bin/bash

if [[ $# -eq 0 ]]
then
      echo "Usage: $(basename $0) [obj_file_list.txt]"
      exit
fi


CURDIR=$(pwd)
OBJ_FILE_LIST=$CURDIR/$1
DEST_FOLDER=~/nir_lib/

[[ ! -d $DEST_FOLDER ]] && mkdir -p $DEST_FOLDER

cd $DEST_FOLDER

ls | xargs -I{} unlink {}

for i in $(cat $OBJ_FILE_LIST)
do
    ln -s $CURDIR/$i .
done
