#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FLAG_SIZE 69+1
#define KEY_STREAM_SIZE 256

typedef unsigned char BYTE;

int g_rnd_val = 0x00;

BYTE* gen_keystream( int seed );
int get_random_value( int seed );

// Cup{b6d820d912d3844ac3f60f5c2441c684bf2ab4dbf6df0f0bc710f9d8b461bc7f}
int _valids[ 69 ] = { 233, 101, 94, 71, 72, 238, 98, 12, 80, 160, 218, 213, 
	43, 122, 18, 151, 234, 52, 26, 61, 233, 139, 128, 34, 114, 22, 171, 
	175, 200, 28, 98, 181, 209, 214, 54, 8, 8, 254, 244, 149, 64, 100, 
	26, 206, 188, 62, 82, 2, 162, 166, 222, 126, 41, 79, 151, 228, 100, 
	9, 58, 180, 216, 220, 32, 117, 16, 195, 249, 154, 87
};

int main()
{
	puts( "=========== WELCOME TO OUR REVERSER PARTY ===========" );
	printf( "Enter private token: " );

	BYTE* buf = (BYTE*) malloc( FLAG_SIZE );
	int nbytes = read( 0, buf, FLAG_SIZE );
	buf[ nbytes - 1 ] = '\0';

	BYTE* key_stream = gen_keystream( nbytes );

	for ( int i = 0; i < nbytes - 1; i++ )
	{
		buf[ i ] ^= key_stream[ i ];
		
		if ( buf[ i ] != _valids[ i ] )
		{
			puts( "[-] Error token!" );
			return -1;
		}
	}

	puts( "[+] Valid token!" );

	return 1;
};

int get_random_value( int seed )
{
	g_rnd_val += seed;
	g_rnd_val += ( ( g_rnd_val * 0x13377331 ) + 0xc00fe ) & 0xff;

	return g_rnd_val;
};

BYTE* gen_keystream( int seed )
{
	BYTE* key_stream = (BYTE*) malloc( KEY_STREAM_SIZE );

	for ( int i = 0; i < KEY_STREAM_SIZE; i++ )
	{
		key_stream[ i ] = get_random_value( i + seed );
	}

	return key_stream;
};