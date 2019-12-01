#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 256   // 2^8

void cipher_swap( BYTE* a, BYTE* b );
int cipher_gen_table( BYTE* key, BYTE* S );
int cipher_encrypt( BYTE* S, BYTE* plaintext, BYTE* ciphertext, int plain_size );
int cipher( BYTE* key, BYTE* plaintext, BYTE* ciphertext, int plain_size  );
