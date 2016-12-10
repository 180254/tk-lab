#!/usr/bin/python3

import os
import sys
import re

# ----------------------------------------------------------------------------

file  = [None, None]
lines = [None, None]

# ----------------------------------------------------------------------------

file[0] = sys.argv[1]
file[1] = sys.argv[2]

# ----------------------------------------------------------------------------

with open(file[0]) as f:
    lines[0] = f.readlines()

with open(file[1]) as f:
    lines[1] = f.readlines()

# ----------------------------------------------------------------------------

fail = False

# ----------------------------------------------------------------------------

if len(lines[0]) != len(lines[1]):
    fail = True
    print("FAIL (-1)")

# ----------------------------------------------------------------------------

comment = re.compile(r";.*")
white = re.compile(r"[ \t]+");

def fix_line(line):
    line = comment.sub("", line)
    line = white.sub(" ", line)
    line = line.strip()
    return line

# ----------------------------------------------------------------------------

lines[0] = [fix_line(line) for line in lines[0]]
lines[1] = [fix_line(line) for line in lines[1]]

# ----------------------------------------------------------------------------

if not fail:
    for i,k in enumerate(lines[0]):
        if lines[0][i] != lines[1][i]:
            fail = True
            print("FAIL (%s)" % (i+1))
            print(lines[0][i])
            print(lines[1][i])
            break

    if not fail:
        print("OK")
