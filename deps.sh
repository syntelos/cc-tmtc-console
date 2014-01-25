#!/bin/bash

if [ "${1}" ]&&[ -d "${1}" ]
then
    dir="${1}"
    egrep '^#include ' ${dir}/*.{h,cpp} | sed 's/.*#include //; s/"//; s/\.h"//;' | sort -u 
else
    cat<<EOF>&2
Usage
    $0 <project-directory>

EOF
    exit 1
fi
