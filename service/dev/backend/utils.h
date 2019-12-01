#include "main.h"

#define MAX_DWORD_VALUE 0xffffffff
#define MAX_WORD_VALUE  0xffff
#define MAX_BYTE_VALUE  0xff

DWORD unpack_32_le( BYTE* );
WORD unpack_16_le( BYTE* );

BYTE* pack_16_le( WORD );
BYTE* pack_32_le( DWORD );

int save_data_in_file( BYTE*, BYTE*, int );
BYTE* read_file( BYTE*, int* );
int get_file_size( BYTE* ); 
