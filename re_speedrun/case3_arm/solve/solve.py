def KSA(key):
    keylength = len(key)

    S = range(256)

    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % keylength]) % 256
        S[i], S[j] = S[j], S[i]  # swap

    return S


def PRGA(S):
    i = 0
    j = 0
    while True:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]  # swap

        K = S[(S[i] + S[j]) % 256]
        yield K


def RC4(key):
    S = KSA(key)
    return PRGA(S)


from_binary = [62,55,59,168,119,80,157,241,181,208, 250,43,32,223,102,94,124,107,37,244,50,94,174,21, 130,197,34,5,159,200,155,75,77,46,227,188,133,42, 81,172,247,213,223,75,228,157,134,156,117,223,115, 49,3,5,1,207,212,232,186,17,112,1,131,66,59,37,43, 4,48]


key = "__malloc_hook"

buf = ''
for i in from_binary:
	buf += chr( i )

plaintext = buf

def convert_key(s):
    return [ord(c) for c in s]
key = convert_key(key)

keystream = RC4(key)

import sys

for c in plaintext:
    sys.stdout.write("%c" % (ord(c) ^ keystream.next()))
