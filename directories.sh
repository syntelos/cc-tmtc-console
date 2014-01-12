#!/bin/bash
#
# Output list of principal project directory name strings without path
# separators
#
for dir in $(find . -type f -name '*.pro' | sed 's%^\./%%; s%/.*%%' | egrep '^[A-Z]' | sort -u )
do
    echo $dir
done
