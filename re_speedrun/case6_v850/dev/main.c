#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int __hooks[] = { 133, 228, 228, 229, 225, 71, 158, 78, 157, 155, 
	73, 80, 82, 92, 170, 83, 174, 91, 161, 80, 88, 190, 112, 162,
	77, 186, 117, 73, 112, 71, 120, 175, 163, 116, 119, 120, 162, 
	155, 78, 72, 77, 75, 102, 102, 73, 92, 92, 109, 89, 100, 90, 
	84, 87, 77, 127, 99, 79, 101, 49, 55, 53, 109, 109, 99, 51, 
	125, 112, 119, 67
};

// Cup{3aec551c7ac3746d9c449fb5582eb834ce89075f5b0b80a5f1f4209ee6b9acdd}

int main( int argc, char* argv[] )
{
	if ( argc <= 1 )
	{
		printf( "[-] Enter the token!\n" );
	}

	int token_size = strlen( argv[ 1 ] );
	int i = 0;

	if ( token_size != 69 )
	{
		printf( "[-] Error token size!\n" );
		return -1;
	}

	for ( i = 0; i < token_size; i++ )
	{
		argv[ 1 ][ i ] += i;
		argv[ 1 ][ i ] ^= i;
	}

	char* reversed_buffer = (char*) malloc( 70 );
	
	for ( i = 68; i >= 0; i-- )
	{
		reversed_buffer[ token_size - i - 1 ] = argv[ 1 ][ i ]; 
	}

	for ( i = 0; i < token_size; i++ )
	{
		if ( reversed_buffer[ i ] != __hooks[ i ] )
		{
			printf( "[-] Token error!\n" );
			return -1;
		}
	}

	printf( "[+] Token valid!\n" );

	return 0;
}