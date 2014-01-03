#!/bin/bash

for dir in $(./directories.sh )
do
    egrep '^#include ".*/' $(find ${dir} -type f -name '*.h' -o -name '*.cpp') | sed 's/.*include "//; s/\/.*//' | sort -u | sed "s/^/${dir}:/"

done
