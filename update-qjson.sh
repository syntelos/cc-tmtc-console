#!/bin/bash

src_dir=~/src/iss-live-qt/QJson
tgt_dir=QJson

for src in $(find ${src_dir} -type f -name '*.h*' -o -name '*.c*' -o -name '*.txt')
do
    tgt=${tgt_dir}/$(basename ${src})
    if [ -f "${tgt}" ]
    then
        if [ "${src}" -nt "${tgt}" ]
        then
            cp -p "${src}" "${tgt}"
            echo "U ${tgt}"
        fi
    else
        if cp -p "${src}" "${tgt}"
        then
            git add "${tgt}"
            echo "A ${tgt}"
        else
            echo "? ${tgt}"
        fi
    fi
done
exit 0
