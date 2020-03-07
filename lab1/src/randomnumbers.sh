#!/bin/bash
if [ -n "$1" ]
then
if [ -n "$2" ]
then
touch $2
for (( i = 0; i < $1; i++ )) 
do 
echo $(od -A n -t d -N 1 /dev/random) >> $2
done
fi
fi