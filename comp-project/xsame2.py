#!/usr/bin/python3

# usage: ./xsame2.py ../vm/gcd.pas

import sys
import os

if os.path.isfile(sys.argv[1]) :
    os.system("../vm/komp %s > /dev/null" % sys.argv[1])
    os.system("./xcomp %s > output2.asm" % sys.argv[1])
    os.system("python3 xsame1.py output.asm output2.asm")
else:
    print("no such file")
