#!/usr/bin/env python2

# Unhex dics with some ascii words and some
# hexcoded words, beginning with $HEX[
# (as seen on TV)
#
# To speed up execution for large files you might first
# want to extract the '^$HEX\[' lines with grep (tm), which
# is faster schmaster than python.
#
# Note: unhexdic only unhexes dics, not hash potfiles.
# To unhex lines, start them with $HEX[
#
# Grep is faster than python for Largo LaGrande files.
# To get better speed, dance with your neighbour or:
#
# O8> rm -f swedish-dic.txt
#
# # Cut words from le.pot into swedish-dic.txt
# O8> cut -d: -f2- le.pot | unique -mem=25 swedish-dic.txt
#
# # Split swedish-dic into nohexbaby and hexylady
# O8> grep -v '^$HEX\[' swedish-dic.txt > nohexbaby.txt
# O8> grep '^$HEX\[' swedish-dic.txt | unhexdic.py > hexylady.txt
#
# Reassemble swedish-dic w/o hex
# O8> rm -f swedish-dic.txt
# O8> cat nohexbaby.txt hexylady.txt | unique -mem=25 swedish-dic.txt
# O8> rm nohexbaby.txt hexylady.txt
#
# Btw, O8> represents scary schmary hexnix prompt, including
#          mandatory tinfoil hat, and unique is part of your
#          very own John the Ripper.
#
# Grep big dics and unhexdic small dics directly.
#
# $HEX[333d3d3d447e7e7e73706c30697421] 
#
# TODO: Write more useless crap in the comments.
#
# All Rights Reversed, Albert "trebla" Veli, Boomtime the
# 68th day of Bureaucracy in the Year of Our Lady Discord
# 3183.

import sys
import re
import binascii
import fileinput

if (len(sys.argv) == 2) and (sys.argv[1] == '-h'):
    print 'Usage:', sys.argv[0], '<dic to unhex>'
    exit(1)

# Prepare regex to find or not find lines beginning w/ $HEX[
regprog = re.compile('^\$HEX\[([0-9a-f]*)')

# Use either stdin or first argument as input
for line in fileinput.input():
    m = regprog.search(line)
    if not m:
        # Print dic-line as is (rm trailing newline)
        print line[:-1]
    else:
        # unhex before printing dic-line
        try:
            b = binascii.unhexlify(m.group(1))
            print b
        except:
            next
