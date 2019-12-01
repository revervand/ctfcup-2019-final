#include "main.h"

uint_least32_t valids[ 69 ] = {
	0x3dd7ffa7,0xf26d6a3e,0x82079eb1,0x15d54739,
	0x1ad5be0d,0x76d32be0,0xfa005713,0x1db87a14,
	0xfa005713,0x6abf4a82,0x76d32be0,0x1ad5be0d,
	0x1ad5be0d,0x1db87a14,0xf4dbdf21,0x6b9df6f,
	0xf4dbdf21,0x1db87a14,0x6b9df6f,0x83dcefb7,
	0x6abf4a82,0xe8b7be43,0x98dd4acc,0x76d32be0,
	0x76d32be0,0xefda7a5a,0x71beeff9,0xf3b61b38,
	0x8d076785,0x84b12bae,0x6dd28e9b,0xfa005713,
	0x6abf4a82,0x76d32be0,0xefda7a5a,0xe8b7be43,
	0xefda7a5a,0x98dd4acc,0x1ad5be0d,0xf3b61b38,
	0x6b9df6f,0xfa005713,0xefda7a5a,0x84b12bae,
	0x98dd4acc,0xf4dbdf21,0xe8b7be43,0x98dd4acc,
	0x8d076785,0x8d076785,0x83dcefb7,0xfa005713,
	0x1ad5be0d,0xfa005713,0x98dd4acc,0x84b12bae,
	0x8d076785,0xf4dbdf21,0x98dd4acc,0x6b9df6f,
	0xf3b61b38,0x6abf4a82,0xf3b61b38,0x1ad5be0d,
	0x8d076785,0xe8b7be43,0x6dd28e9b,0x84b12bae,
	0xfcb6e20c
};

uint_least32_t Crc32( unsigned char *buf, size_t len )
{
    uint_least32_t crc_table[ 256 ];
    uint_least32_t crc; int i, j;

    for ( i = 0; i < 256; i++ )
    {
        crc = i;
        for ( j = 0; j < 8; j++ )
            crc = crc & 1 ? ( crc >> 1 ) ^ 0xEDB88320UL : crc >> 1;

        crc_table[ i ] = crc;
    };

    crc = 0xFFFFFFFFUL;

    while ( len-- )
        crc = crc_table[ (crc ^ *buf++ ) & 0xFF ] ^ ( crc >> 8 );

    return crc ^ 0xFFFFFFFFUL;
}

int main()
{
	// char buffer[ 69 ] = "Cup{2f8687f2260c06c17adffeb495387feaed24c8e5d0ad991828d590dc47429a35}";

	unsigned char* buffer = (unsigned char*) malloc( 70 );

	printf( "{?} Enter the flag: " );
	
	int nbytes = read( 0, buffer, 70 );
	buffer[ nbytes - 1 ] = '\0';

	if ( strlen( (const char*)buffer ) != 69  )
	{
		puts( "{-} Invalid flag!" );
	}

	for ( int i = 0; i < 69; i++ )
	{
		if ( Crc32( &buffer[ i ], 1 ) != valids[ i ] )
		{
			puts( "{-} Invalid flag!" );
			return -1;
		}
	}
	
	puts( "{+} Valid flag!" );

	return 0;
}