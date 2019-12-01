#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int hash[] = { 66, 115, 112, 117, 57, 57, 99, 27, 95, 41, 103, 
	101, 95, 50, 97, 50, 97, 43, 41, 87, 53, 107, 51, 32, 95, 
	44, 96, 91, 88, 90, 98, 52, 99, 46, 49, 45, 45, 103, 44, 
	27, 49, 96, 52, 99, 47, 98, 97, 44, 93, 42, 48, 52, 59, 
	45, 57, 111, 97, 48, 44, 90, 101, 47, 50, 47, 53, 51, 97, 41, 112
};

int xor_keys[ 16 ] = { 1, 1, 2, 3, 5, 8, 13, 21, 1, 3, 3, 7, 3, 1, 3, 3 };

// Cup{86d7f1dab4f4f33c0d86b2eecba6d0716f910b5a0fd0d226880af47df17189e3}

int main()
{
	char token[ 128 ];

	printf( "[?] Enter the secret token: " );
	scanf( "%127s", token );

	if ( strlen( token ) != 69 )
	{
		printf( "[?] Invalid token!" );
		return -1;
	}

	for ( int i = 0; i < 69; i++ )
	{
		token[ i ] ^= xor_keys[ i % 16 ];
		token[ i ] -= i % 10;

		if ( token[ i ] != hash[ i ] )
		{
			printf( "[-] Incorrect token!\n" );
			return -1;
		}
	}

	printf( "[+] Valid token!\n" );

	return 0;
}