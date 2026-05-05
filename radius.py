#!/usr/bin/env python3
#
# Decrypts PAP (Password Authentication Protocol) credentials from RADIUS
# Access-Request packets captured in pcap/pcapng files.
#
# Only PAP is supported. EAP (EAP-MD5, EAP-TLS, etc.) and CHAP store a hash
# or challenge-response instead of the password, so they cannot be decrypted
# this way. The RADIUS shared secret must be known.
#
# PAP password encryption is defined in RFC 2865, Section 5.2.
# Passwords up to 128 bytes are supported via chained MD5 blocks.

from scapy.all import rdpcap, Radius
from sys import argv
from hashlib import md5

if len(argv) < 3:
    print('Usage: ./radius.py <shared password> <pcap files>')
    raise SystemExit(1)

RADIUS_CODE_ACCESS_REQUEST = 1
RADIUS_ATTR_USERNAME       = 1
RADIUS_ATTR_USER_PASSWORD  = 2

def decrypt_pap_password(shared_secret, authenticator, ciphertext):
    """Decrypt PAP User-Password per RFC 2865 section 5.2 (chained MD5)."""
    plaintext = bytearray()
    prev = authenticator
    for i in range(0, len(ciphertext), 16):
        block = ciphertext[i:i+16]
        keystream = md5(shared_secret + prev).digest()
        plaintext.extend(a ^ b for a, b in zip(block, keystream))
        prev = block
    return bytes(plaintext).rstrip(b'\x00')

shared_secret = argv[1].encode()

for filename in argv[2:]:
    for packet in rdpcap(filename):
        if Radius not in packet:
            continue
        radius_pkt = packet[Radius]
        if radius_pkt.code != RADIUS_CODE_ACCESS_REQUEST:
            continue

        attrs         = radius_pkt.attributes
        authenticator = radius_pkt.authenticator

        username_raw = next((a.value for a in attrs if a.type == RADIUS_ATTR_USERNAME), None)
        passhash     = next((a.value for a in attrs if a.type == RADIUS_ATTR_USER_PASSWORD), None)

        if username_raw is None or passhash is None:
            continue  # not PAP or malformed

        password = decrypt_pap_password(shared_secret, authenticator, passhash)

        username = username_raw.decode(errors='replace')
        password_str = password.decode(errors='replace')

        print(f'username: {username}')
        print(f'password: {password_str}')
