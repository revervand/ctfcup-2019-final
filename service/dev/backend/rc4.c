#include "rc4.h"

void cipher_swap( BYTE* a, BYTE* b ) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
};

int cipher_gen_table( BYTE* key, BYTE* S ) 
{
    int len = strlen( key );
    int j = 0;

    for ( int i = 0; i < N; i++ )
        S[ i ] = i;

    for ( int i = 0; i < N; i++ ) 
    {
        j = ( j + S[ i ] + key[ i % len ] ) % N;

        cipher_swap( &S[ i ], &S[ j ] );
    }

    return 0;
};

int cipher_encrypt( BYTE* S, BYTE* plaintext, BYTE* ciphertext, int plain_size ) 
{
    int i = 0;
    int j = 0;

    for ( size_t n = 0, len = plain_size; n < len; n++ ) 
    {
        i = ( i + 1 ) % N;
        j = ( j + S[ i ] ) % N;

        cipher_swap( &S[ i ], &S[ j ] );
        int rnd = S[ ( S[ i ] + S[ j ] ) % N ];

        ciphertext[ n ] = rnd ^ plaintext[ n ];
    }

    return 0;
};

int cipher( BYTE* key, BYTE* plaintext, BYTE* ciphertext, int plain_size ) 
{
    BYTE S[ N ];
    cipher_gen_table( key, S );

    cipher_encrypt( S, plaintext, ciphertext, plain_size );

    return 0;
};

// int main(int argc, char *argv[]) {

//     if(argc < 3) {
//         printf("Usage: %s <key> <plaintext>", argv[0]);
//         return -1;
//     }

//     unsigned char *ciphertext = malloc(sizeof(int) * strlen(argv[2]));

//     RC4(argv[1], argv[2], ciphertext);

//     for(size_t i = 0, len = strlen(argv[2]); i < len; i++)
//         printf("%02hhX", ciphertext[i]);

//     return 0;
// }