import os
import sys
import re

file  = [None, None]
lines = [None, None]

file[0] = sys.argv[1]
file[1] = sys.argv[2]

with open(file[0]) as f:
    lines[0] = f.readlines()

with open(file[1]) as f:
    lines[1] = f.readlines()

if len(lines[0]) != len(lines[1]):
    print("FAIL (-1)")
    sys.exit(0)

comment = re.compile(r";.*")
white = re.compile(r"[ \t]+");

def fix_line(line):
    line = comment.sub("", line)
    line = white.sub(" ", line)
    line = line.strip()
    return line

fail = False

for i,k in enumerate(lines[0]):
    if fix_line(lines[0][i]) != fix_line(lines[1][i]):
        fail = True
        print("FAIL (%s)" % (i+1))
        break

if not fail:
    print("OK")
