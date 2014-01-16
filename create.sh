#!/bin/bash

if [ -n "${1}" ]&&[ -n "$(echo ${1} | egrep '[A-Za-z]*/[A-Za-z]*')" ]
then
    file="${1}"
    dir=$(dirname "${file}")
    class=$(basename "${file}")
    header=${file}.h
    source=${file}.cpp
    define="_CONSOLE_$(echo $dir | tr 'a-z' 'A-Z')_${class}_H"
    cp HEAD.txt ${header}
    cat<<EOF >> ${header}
#ifndef $define
#define $define

/*!
 * 
 */
class ${class} {

 public:
    ${class}();
    ~${class}();

};
#endif
EOF
    cp HEAD.txt ${source}
    cat<<EOF >> ${source}

#include "${class}.h"

${class}::${class}()
{
}
${class}::~${class}()
{
}
EOF
    ls -l $source $header
    git add $source $header
else
    cat<<EOF>&2
usage: $0 <Dir/BaseName>
EOF
    exit 1
fi
