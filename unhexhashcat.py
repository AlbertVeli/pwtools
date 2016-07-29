#!/usr/bin/env python

# Unhex hex-lines from hashcat.pot
#
# All Rights Reversed
# Albert Veli, Setting Orange, the 64th day of Confusion in the YOLD 3182

import sys
import re
import binascii

# Get filename from cmdline
if len(sys.argv) != 2:
    print 'Usage:', sys.argv[0], '<filename>'
    exit(1)
else:
    fname = sys.argv[1]

try:
    fp = open(fname, 'r')
except IOError:
    print 'Error: couldn\'t open file', fname, 'for reading'
    exit(1)

# Prepare regex.
# Find lines containing patterns like this:
# $HEX[67756c6772c3b66e]
# The line in hashcat.pot has hash and : too, but
# the regex filters that out and isolates the
# hexdigits into m.group(1).
regprog = re.compile('\$HEX\[([0-9a-f]*)')

# Loop through all lines in file
for line in fp:
    # Find lines matching the pattern
    m = regprog.search(line)
    if not m:
        next
    # unhex and print to stdout
    try:
        b = binascii.unhexlify(m.group(1))
        print b
    except:
        next

fp.close()
