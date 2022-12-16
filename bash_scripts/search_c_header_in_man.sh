#!/usr/bin/env bash

MANPATH=$(manpath)

function FindCHeader() {
    echo $1 | sed -ne 's/^.* <\([a-zA-Z\.]*\)>/\1/p'
}

function FindInclude() {
    IFS=":"
    count=0
    for dir in $MANPATH; do
        compressed_page="$dir/man3/$1.3.gz"
        uncompressed_page="$dir/man3/$1.3"

        if [ -f $compressed_page ]; then
            FindCHeader "$(gunzip -c $compressed_page)" && return 0
        elif [ -f $uncompressed_page ]; then
            FindCHeader "$(cat $uncompressed_page)" && return 0
        fi

    done

    return 1
}

while read command && [ -n "${command}" ]; do
    FindInclude $command || echo "---"
done
