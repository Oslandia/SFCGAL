#!/bin/bash

base=$(dirname $(dirname $0)) # we should be in SFCAGL/script

if [ ! -f "$base/src/version.h.cmake" ]; then
    echo "Can't find $base/src/version.h.cmake"
    exit 1
fi

for a in $@; do
    case $a in
        --fix)
            fix=true;;
        -*)
            echo unknown option $a
            exit 1;;
        *)
            file=$a;;
    esac
done


err=""
for dir in "$base/src" "$base/test" "$base/viewer"; do
    #echo processing $dir
    for src in $(find $dir -name '*.cpp' -or -name '*.h'); do
        #echo "   " $src
        astyle -n --style=stroustrup --indent=spaces=4 \
               --break-closing-brackets --break-blocks --align-pointer=type \
               --add-brackets --pad-paren-in < $src > astyle.tmp.out 
        dif=$(diff $src astyle.tmp.out)
        if [ -n "$dif" ]; then
            #echo diff in $src
            if [ $fix ]; then
                cp astyle.tmp.out $src
            else
                err="$err $src"
            fi
        fi
    done
done

n=0
for e in $err; do
    echo "diff in $e"
    n=$(($n + 1))
done

if [ -n "$err" ]; then
    echo "******* $n errors found"
    exit 1
fi
