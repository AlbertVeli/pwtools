#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Run JohnTheRipper or hashcat repeatedly
# with different masks. Specify mask file
# on cmdline. One mask per line.
#
# Note. Hashcat already supports to load a
# masks file, usually with .hcmask extension.
#
# Advantage of running this script is that
# you can abort execution and continue later.
# Already run masks are kept in a separate file.
#
###
#
#            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#                    Version 2, December 2004
# 
# Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
#
# Everyone is permitted to copy and distribute verbatim or modified
# copies of this license document, and changing it is allowed as long
# as the name is changed.
# 
#            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
#
#  0. You just DO WHAT THE FUCK YOU WANT TO.
#
###
#
# Pungenday, the 67th day of Confusion in the YOLD 3184, Albert Veli
#
###

import subprocess
import sys
import multiprocessing

# If runmasks_config exists, load variables from there
# else use default values.
import os.path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
config = 'runmasks_config.py'
if os.path.isfile(config):
    from runmasks_config import *
else:
    # Default values.
    # Put these in runmasks_config.py to customize.
    john = 'john'
    john_mode = 'Raw-SHA1'
    use_fork = True
    use_john = True
    hc = 'hashcat'
    hc_mode = '100'
    hc_outfile = 'cracked.txt'
    john_potfile = 'myjohn.pot'
    hc_potfile = 'myhc.pot'
    hashfile = 'hashes.sha1'

# Hashcat/john mask commands, add mask at end
hc_mask_cmd = [hc, '--session=hcmask', '-O',
               '--outfile-format', '2', '-o', hc_outfile,
               '-m', hc_mode, '--potfile-path', hc_potfile,
               hashfile, '-a3']

# John
john_mask_cmd= [john, '--format:' + john_mode, '--pot:' + john_potfile,
        '--session:johnmask', hashfile]

# Run command and return retcode.
# Ctrl-C is captured and sent to subprocess.
def run_cmd(cmd):
    # cmd is a list of command and arguments
    try:
        #result = subprocess.run(cmd)
        p = subprocess.Popen(cmd)
        print('DBG: pid =', p.pid)
        p.communicate()
    except KeyboardInterrupt:
        print('\nKilled by Ctrl-C')
        # 0 or 1 is success, return something else
        return 666
    return p.returncode

def run_hashcat_mask(mask):
    # Add mask to hc_mask_cmd and run
    cmdline = hc_mask_cmd + [mask]
    retcode = run_cmd(cmdline)

    # retcodes:
    # 0 = everything cracked
    # 1 = finished, not all cracked
    # 2 = quit by q
    # 255 = Other error
    # 666 = Killed by Ctrl-C
    # Remove debug line when everything works
    if (retcode != None) and (retcode < 2):
        return True

    return None

def run_john_mask(mask):
    # Add mask to hc_mask_cmd and run
    cmdline = john_mask_cmd + ['--mask:' + mask]
    # Add use_fork = True to runmasks_config.py to fork
    if use_fork:
        cmdline += ['--fork:' + str(multiprocessing.cpu_count())]

    # Note, I didn't manage to figure out tty/pty settings
    # so that s (for status) and q (for quit) are
    # sent on to john. For now, press Ctrl-C to quit.
    # To get status, send SIGHUP to pid from separate
    # terminal. Pid is printed at startup.
    retcode = run_cmd(cmdline)

    # retcodes:
    if (retcode != None) and (retcode < 2):
        return True

    if retcode == 666:
        sys.exit(1)

    return None


# --- main

# Load masks
print(sys.argv[0])
if len(sys.argv) != 2:
    print('Usage:', sys.argv[0], '<mask file>')
    sys.exit(1)

mask_file = sys.argv[1]
allmasks = [line.rstrip() for line in open(mask_file)]

# Load done
done_file = mask_file + '_done'
if os.path.isfile(done_file):
    donemasks = [line.rstrip() for line in open(done_file)]
else:
    donemasks = []

# Run john (or hashcat) on remaining masks
f = open(done_file, 'a')
for mask in allmasks:
    if mask in donemasks:
        print('Skipping done mask', mask)
    else:
        print('Running mask:', mask)
        if use_john:
            ret = run_john_mask(mask)
        else:
            ret = run_hashcat_mask(mask)
        if ret:
            donemasks.append(mask)
            f.write(mask + '\n')

f.close()
