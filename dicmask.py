#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Dictionary mask analyser.
# Does the same as pwgen_stat but
# this is easier to understand and
# as a bonus it does sorting automatically.
#
# EXAMPLES
#
# Generate length 8 masks from rockyou:
# dicmask.py --min 8 --max 8 rockyou.txt > rockyou_8.txt
#
# Fetch 100 most common patterns:
# head -100 rockyou_8.txt > rockyou_8_top100.txt
#
# The file should look something like this:
# ?l?l?l?l?l?l?l?l
# ?d?d?d?d?d?d?d?d
# ?l?l?l?l?l?l?d?d
# ?l?l?l?l?d?d?d?d
# ?l?l?l?l?l?l?l?d
# ?l?l?l?l?l?d?d?d
# ?l?l?d?d?d?d?d?d
# ?u?u?u?u?u?u?u?u
# ...
#
# Loop through mask patterns:
# for mask in `cat rockyou_8_top100.txt`; do
#     ./john ... --mask:$mask hashes.txt
# done
#
# To use hashcat instead, replace john line with:
# hashcat ... hashes.txt -a 3 $mask
#
# LICENSE
#
# Do whatever you want with this software, but don't
# blame me if your GPU catches fire. Seriously. Checkout
# the --gpu-temp-abort argument to hashcat.
# (UNLICENSE, without so many words)
#
# Boomtime, the 56th day of Confusion in the YOLD 3184
#
# Albert Veli
# -----------

import operator
import argparse
import codecs

lower_chars = list('abcdefghijklmnopqrstuvwxyz')
upper_chars = list('ABCDEFGHIJKLMNOPQRSTUVWXYZ')
digit_chars = list('0123456789')
special_chars = list('!"#$%&\'()*+,-./:;<=>?@[\]^_`{|}~')

# Take a word, return None or mask string
def build_mask(word):
    # Check min/max length
    l = len(word)
    if args.min and l < int(args.min):
        # Too short
        return None
    if args.max and l > int(args.max):
        # Too long
        return None

    # Calculate mask
    mask = ''
    for c in word:
        if c in lower_chars:
            mask += '?l'
        elif c in upper_chars:
            mask += '?u'
        elif c in digit_chars:
            mask += '?d'
        elif c in special_chars:
            mask += '?s'
        else:
            # F*ck this word and its utf8 characters
            return None
    return(mask)

# Loop through lines in filename and calucalte masks.
# Also sort the masks before printing them out.
def print_masks(filename):
    masks = {}
    with codecs.open(filename, 'r', encoding='utf-8', errors='ignore') as f:
        maskdic = {}
        for line in f:
            # Remove newline
            line = line.rstrip()
            m = build_mask(line)
            # m is None if min/max length doesn't match
            # or if it contains non-ascii characters.
            if m:
                if m in masks:
                    # Mask exists, increase count
                    masks[m] += 1
                else:
                    # Create new mask with count 1
                    masks[m] = 1

    # Magic sorting line (from Stack Overflow...)
    sorted_masks = sorted(masks.items(), key=operator.itemgetter(1))
    # Reverse so list is in descending order
    sorted_masks.reverse()
    for mask in sorted_masks:
        # mask[0] is the mask string and
        # mask[1] is the count
        if args.count:
            print(mask[0], mask[1])
        else:
            print(mask[0])

# --- Main ---

# Custom info lines
info =  'Loop through all lines of dictionary (aka wordlist)\n'
info += 'and print out JtR/hashcat masks. Most common first.\n\n'
info += '  ?l = lowercase character\n'
info += '  ?u = upperase character\n'
info += '  ?d = digit character\n'
info += '  ?s = special character\n\n'
info += '?a = all of the above. Not used. Figure out yourself\n'
info += 'how to combine multiple lines using ?a.\n\n'
info += 'EXAMPLE\n\nPrint length 8 masks in rockyou.txt\n\n'
info += '  %(prog)s --min 8 --max 8 rockyou.txt\n'

parser = argparse.ArgumentParser(usage='%(prog)s [options] <dic>' + '\n\n' + info + '\n')
# Optional arguments
parser.add_argument('--min', help='Minimum word length to examine', nargs='?')
parser.add_argument('--max', help='Maximum word length to examine', nargs='?')
parser.add_argument('--print-count', dest='count', help='Print out mask count together with mask', action='store_true')

# Mandatory arguments
parser.add_argument('dic', help='Wordlist file to examine', nargs='+')

args = parser.parse_args()

# Multiple dictionaries can also be given.
#
# dicmask.py --min 9 --max 9 rockyou.txt hibp-top-1M.txt
#
# Unnecessary wall of text
#
# This will present masks individually from each list.
# Merge lists to get summary statistic masks.
#
# cat rockyou.txt hibp-top-1M.txt | unique big.dic
#
# unique comes with JtR and is faster than the standard
# unix tools at creating unique lines from big inputs.
# Note. You can also use rli2 or mli2 from hashcat-utils
# if you sort the dics first:
#
# cat dic1.txt | LANG=C sort -u > sorted1.dic
# cat dic2.txt | LANG=C sort -u > sorted2.dic
# mli2 sorted1.dic sorted2.dic > big.dic
#
# dicmask.py --min 9 --max 9 big.dic
#
# unique doesn't require sorting first.

for dic in args.dic:
    if len(args.dic) > 1:
        print('\n' + dic + ':\n')
    print_masks(dic)
