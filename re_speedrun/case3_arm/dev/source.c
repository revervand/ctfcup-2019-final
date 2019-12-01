#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 256   // 2^8

// Cup{335e2cba8ae5b58589e644e64338048648bcea136385ce6640e7d7deb1d4c9d5}
int valids[69] = {62,55,59,168,119,80,157,241,181,208,
    250,43,32,223,102,94,124,107,37,244,50,94,174,21,
    130,197,34,5,159,200,155,75,77,46,227,188,133,42,
    81,172,247,213,223,75,228,157,134,156,117,223,115,
    49,3,5,1,207,212,232,186,17,112,1,131,66,59,37,43,
    4,48
};

void swap(unsigned char *a, unsigned char *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int KSA(char *key, unsigned char *S) {

    int len = strlen(key);
    int j = 0;

    for(int i = 0; i < N; i++)
        S[i] = i;

    for(int i = 0; i < N; i++) {
        j = (j + S[i] + key[i % len]) % N;

        swap(&S[i], &S[j]);
    }

    return 0;
}

int PRGA(unsigned char *S, char *plaintext, unsigned char *ciphertext) {

    int i = 0;
    int j = 0;

    for(size_t n = 0, len = strlen(plaintext); n < len; n++) {
        i = (i + 1) % N;
        j = (j + S[i]) % N;

        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % N];

        ciphertext[n] = rnd ^ plaintext[n];

    }

    return 0;
}

int RC4(char *key, char *plaintext, unsigned char *ciphertext) {

    unsigned char S[N];
    KSA(key, S);

    PRGA(S, plaintext, ciphertext);

    return 0;
}

int main(int argc, char *argv[]) {

    if ( argc < 2 )
    {
        printf( "Usage: %s <flag>\n", argv[ 0 ] );
    }

    unsigned char *ciphertext = malloc( sizeof(int) * strlen( argv[ 1 ] ) );

    RC4( "__malloc_hook", argv[ 1 ], ciphertext );

    for ( int i = 0; i < strlen( argv[ 1 ] ); i++ )
    {
        if ( ciphertext[ i ] != valids[ i ] )
        {
            puts( "{-} Invalid flag!" );
            return -1;
        }
    }

    puts( "{+} Valid flag!" );

    return 0;
}