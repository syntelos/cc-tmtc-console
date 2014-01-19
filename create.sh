#!/bin/bash

emit_header=true
emit_source=true
file=""

#
# INIT
#
for arg in $*
do
    case ${arg} in
        --source|--header)
            #
            # Convert semantics of source and header to positive assertions
            #
            emit_source=false
            emit_header=false
            ;;
    esac
done

#
# Process cmdln arguments
#
for arg in $*
do
    if [ -n "$(echo ${arg} | egrep '[A-Za-z]*/[A-Za-z]*')" ]
    then
        file="${arg}"
    else
        case ${arg} in
            --source)
                if ${emit_source}
                then
                    emit_source=false
                else
                    emit_source=true
                fi
                ;;
            --header)
                if ${emit_header}
                then
                    emit_header=false
                else
                    emit_header=true
                fi
                ;;
        esac
    fi
done

#
# Process input
#
if [ -n "${file}" ]
then
    dir=$(dirname "${file}")
    class=$(basename "${file}")
    header=${file}.h
    source=${file}.cpp
    define="_CONSOLE_$(echo $dir | tr 'a-z' 'A-Z')_${class}_H"
    #
    if ${emit_header}
    then
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
        ls -l ${header}
        git add ${header}
    fi
    #
    if ${emit_source}
    then
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
        ls -l ${source}
        git add ${source}
    fi

else
    cat<<EOF>&2
Usage

    $0 <Dir/BaseName> [--source|--header]

Description

    Emit skeletal source and header, or optionally select one of
    source or header to write to file.

EOF
    exit 1
fi
