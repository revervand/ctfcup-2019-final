#include "parser.h"
#include "utils.h"
#include "err.h"
#include "engine.h"

int parse_packet( BYTE* packet )
{
	BYTE* tmp_buffer = (BYTE*) calloc( MAX_PACKET_SIZE, 
		sizeof( BYTE ) );

	int packet_offset = 0;
	// get real packet size
	memcpy( tmp_buffer, packet, sizeof( DWORD ) );
	DWORD real_size = unpack_32_le( tmp_buffer );

	if ( real_size == MAX_DWORD_VALUE || real_size > MAX_PACKET_SIZE )
	{
		send_error( PACKET_SIZE_ERR );
		return PACKET_SIZE_ERR;
	}

	packet_offset += 4;

	memset( tmp_buffer, 0, MAX_PACKET_SIZE );
	memcpy( tmp_buffer, packet + packet_offset, sizeof( WORD ) );

	struct command parsed_cmd;

	parsed_cmd.id = unpack_16_le( tmp_buffer );

	free( tmp_buffer );
	
	packet_offset += 2;

	parsed_cmd.payload = &packet[ packet_offset ];
	parsed_cmd.size = real_size - sizeof( parsed_cmd.id );

	int errcode = run_command( &parsed_cmd );

	return TRUE;
};