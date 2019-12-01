#include "utils.h"

DWORD unpack_32_le( BYTE* buf )
{
	DWORD value = MAX_DWORD_VALUE;

	value  = ( (BYTE) buf[ 3 ] ) << 24;
	value += ( (BYTE) buf[ 2 ] ) << 16;
	value += ( (BYTE) buf[ 1 ] ) << 8;
	value += ( (BYTE) buf[ 0 ] );
	
	return value;
};

WORD unpack_16_le( BYTE* buf )
{
	WORD value = MAX_WORD_VALUE;

	value  = ( (BYTE) buf[ 1 ] ) << 8;
	value += ( (BYTE) buf[ 0 ] );
	
	return value;
};

BYTE* pack_16_le( WORD value )
{
	BYTE* buf = (BYTE*) calloc( sizeof( WORD ), sizeof( BYTE ) );

	buf[ 0 ] = (BYTE) ( value & 0xff );
	buf[ 1 ] = (BYTE) ( value >> 8 );

	return buf;
};

BYTE* pack_32_le( DWORD value )
{
	BYTE* buf = (BYTE*) calloc( sizeof( DWORD ), sizeof( BYTE ) );

	buf[ 0 ] = (BYTE) ( value & 0xff );
	buf[ 1 ] = (BYTE) ( value >> 8 );
	buf[ 2 ] = (BYTE) ( value >> 16 );
	buf[ 3 ] = (BYTE) ( value >> 24 );

	return buf;
};

int save_data_in_file( BYTE* Filename, BYTE* Data, int DataSize )
{
	FILE* fp;
	fp = fopen( Filename, "wb" );

	if ( fp == NULL )
	{
		return FALSE;
	}

	int nbytes = fwrite( (void*) Data, sizeof( BYTE ), DataSize, fp );

	if ( nbytes != DataSize )
		return FALSE;

	fclose( fp );

	return TRUE;
};

int get_file_size( BYTE* Filename ) 
{  
    FILE* fp = fopen( Filename, "r" ); 
  
    if ( fp == NULL ) 
    { 
        return -1; 
    } 
  
    fseek( fp, 0, SEEK_END ); 
  
    int size = ftell( fp ); 
  
    fclose( fp ); 
  
    return size; 
} 

BYTE* read_file( BYTE* Filename, int* size )
{
    *size = get_file_size( Filename );

    if ( *size == -1 )
    	return NULL;

	BYTE* file_data = (BYTE*) calloc( *size, sizeof( BYTE ) );

	FILE* fp = fopen( Filename, "rb" );

	if ( fp == NULL ) 
    {
    	free( file_data ); 
        return NULL; 
    }
 
    int nbytes = fread( file_data, sizeof( BYTE ), *size, fp );

    if ( nbytes != *size )
    {
    	free( file_data );	
    	return NULL;
    }

    return file_data;
};
