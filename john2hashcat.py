#!/usr/bin/env python

# Un-base64 lines from john.pot to hashcat "plain hex" format.
#
# All Rights Reversed
# Albert Veli, Setting Orange, the 64th day of Confusion in the YOLD 3182

import sys
import base64
import re

if len(sys.argv) != 2:
    print 'Usage:', sys.argv[0], '<john hashfile>'
    exit(1)
else:
    infile = sys.argv[1]

fp = open(infile, 'r')

# Find base64 part of lines like
# {SHA}fEqNCco3Yq9h5ZUglD3CZJT4lBs=:123456
# and
# ?:{SHA}fEqNCco3Yq9h5ZUglD3CZJT4lBs=
# The regex must work in both cases
regprog = re.compile('{[A-Z]*}([a-zA-Z0-9=/+]*)')

for line in fp:
    m = regprog.search(line)
    if m:
        binstr = base64.b64decode(m.group(1))
        print ''.join('{:02x}'.format(ord(x)) for x in binstr)
