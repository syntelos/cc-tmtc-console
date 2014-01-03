#!/bin/bash

for dir in $(./directories.sh )
do
    for file in $(find ${dir} -type f -name '*.h' -o -name '*.cpp')
    do
        if [ -z "$(egrep $(basename $file) $dir/${dir}.pro )" ]
        then
            echo Missing ${file}
        fi
    done
done
