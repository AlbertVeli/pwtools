#!/usr/bin/env python2

# Unhex hex-lines from hashcat.pot
#
# To speed up execution for large files you might first
# extract the $HEX[ lines with grep and then unhex them:
#
# grep '\$HEX\[' hashcat.pot > hex.txt
# unhexhashcat.py hex.txt > words.txt
#
# Grep is much faster than both bython and sed for large files,
# that is why the two commands above are faster than running
# unhexhashcat.py on the whole potfile. Then the rest of the
# words can be extracted from the potfile
# like this:
#
# grep -v '\$HEX\[' hashcat.pot | cut -c 42- > nohexwords.txt
#
# where 42 is the number of characters to cut off the beginning. It
# depends on the hash type. 42 for sha1, 34 for md5 etc.
#
# Replace cut with: sed 's/^[^:]*://'
# for a general, but slower solution.
#
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
