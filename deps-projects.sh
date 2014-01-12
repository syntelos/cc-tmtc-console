#!/bin/bash

tmpp=/tmp/$(basename $0 .sh)
tmpd=${tmpp}.$$

if mkdir ${tmpd}
then
    #
    # List principal project directory name strings without path separators
    #
    directories=$(./directories.sh )
    #
    # List dependencies
    #
    for dirn in ${directories}
    do
        tgt=${tmpd}/${dirn}

        egrep '^#include ".*/' $(find ${dirn} -type f -name '*.h' -o -name '*.cpp') | sed 's/.*include "//; s/\/.*//' | sort -u > ${tgt}
    done
    #
    # Sort dependents into order
    #
    for dependent in $(wc -l $(echo ${directories} | sed "s%^%${tmpd}/%; s% % ${tmpd}/%g") | awk '{print $1":"$2}' | egrep -v '(^0:|:total$)' | sort -r | sed 's%.*/%%')
    do
        tgt=${tmpd}/${dependent}

        cat ${tgt} | sed "s/^/${dependent}:/"
    done
    #
    # List independents with lexical order
    #
    for independent in $(wc -l $(echo ${directories} | sed "s%^%${tmpd}/%; s% % ${tmpd}/%g") | awk '{print $1":"$2}' | egrep '^0:' | sed 's%.*/%%')
    do
        echo ${independent}
    done

    rm -rf ${tmpd}

else
    cat<<EOF>&2
$0: error: failed to create temporary directory '${tmpd}'.
EOF
    exit 1
fi
