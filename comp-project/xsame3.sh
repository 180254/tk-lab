#!/bin/bash

for f in ../vm/*.pas; do
    echo -n "$f: "
    python3 xsame2.py $f
done
