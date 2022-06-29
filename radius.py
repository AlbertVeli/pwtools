#!/usr/bin/env python3

from scapy.all import *
from binascii import hexlify
from sys import argv
from hashlib import md5

if len(argv) < 3:
    print('Usage: ./radius.py <shared password> <pcap files>')
    exit(1)

# b1 and b2 must be of equal length
def byte_xor(b1, b2):
    return bytes([_a ^ _b for _a, _b in zip(b1, b2)])

# valid for both login attempts
shared_secret = argv[1].encode()

for filename in argv[2:]:
    for packet in rdpcap(filename):
        if Radius in packet:
            if packet[Radius].code == 1:
                # Access-Request
                authenticator = packet[Radius].authenticator
                keystream = md5(shared_secret + authenticator).digest()
                # keystream is 16 bytes, if password is < 16 characters we are good to go
                username = packet[Radius].attributes[0].value
                passhash = packet[Radius].attributes[1].value
                password = byte_xor(passhash, keystream)
                #print(username, password, password.decode())
                print('username: ' + username.decode())
                print('password: ' + password.decode())
