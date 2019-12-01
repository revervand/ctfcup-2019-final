#include "main.h"
#include "parser.h"
#include "db.h"

struct User g_User;

int main( int argc, char* argv[], char* envp[] )
{
	setup();
	init_user();

	if ( !check_db() )
	{
		init_db();
	}
	
	BYTE* packet = (BYTE*) calloc( MAX_PACKET_SIZE, 
		sizeof( BYTE ) );

	read( 0, packet, MAX_PACKET_SIZE );
	// memcpy( packet, "\x16\x00\x00\x00", 4 ); // all packet size 32 byte
	// memcpy( packet + 4, "\x17\x7e", 2 ); // reg command 32 - 2 = 30
	// memcpy( packet + 6, "\x06", 1 ); // 30 - 1 = 29  username size 
	// memcpy( packet + 7, "revker", 6 );
	// memcpy( packet + 13, "\x06", 1 );
	// memcpy( packet + 14, "kerrev", 6 );
	// memcpy( packet + 20, "\x05", 1 ); 
	// memcpy( packet + 21, "%%%%%", 5 );
	// memcpy( packet + 31, "\x07", 1 );
	// memcpy( packet + 32, "private", 7 );
	//memcpy( packet + 38, "\x10", 1 );
	// memcpy( packet + 39, "aolkeklolkeklol1", 16 );

	// // memcpy( packet + 7, "cesTUseR", 8 ); // 29 - 8 = 21 username 
	// // memcpy( packet + 15, "\x06", 1 ); // 21 - 1 = 20 status size 
	// // memcpy( packet + 16, "Viewer", 6 ); // status 
	// // memcpy( packet + 22, "\x01", 1 ); // token size 
	// memcpy( packet + 7, "e", 1 ); // token

	int errcode = parse_packet( packet );

	free( packet );
	packet = NULL;

	return 0;
};

void setup( void )
{
	alarm( DEFAULT_ALARM_TIME );

	setvbuf( stdout, 0, 2, 0 );
	setvbuf( stderr, 0, 2, 0 );
	setvbuf( stdin,  0, 2, 0 );
};

void init_user( void )
{
	g_User.name = NULL;
	g_User.password = NULL;
	g_User.status = 0;
	g_User.id = 0;
	g_User.session_token = NULL;
};
