#!/bin/bash
count=0
for param in $@
do
count=$(( $count + $param ))
done
echo $(( $count / $# ))
echo $#