#!/bin/bash

# usage: ./xsame3.sh

for f in ../vm/*.pas; do
    echo -n "$f: "
    python3 xsame2.py $f
done
