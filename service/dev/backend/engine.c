#include "engine.h"

int send_error( int _errno )
{
	BYTE* message = (BYTE*) calloc( ERR_MSG_SIZE, sizeof( BYTE ) );

	int message_offset = 0;

	memcpy( message + message_offset, ERR_MSG_IDENT, sizeof( DWORD ) );
	message_offset += sizeof( DWORD );
	memcpy( message + message_offset, (void*)&_errno, sizeof( _errno ) );

	write( 1, message, ERR_MSG_SIZE );

	free( message );
	message = NULL;

	return TRUE;
};

int run_command( struct command* cmd )
{
	switch( cmd->id )
	{
		case REGISTER_COMMAND:
		{
			registration( cmd );
			break;
		};

		case LOGIN_COMMAND:
		{
			login( cmd, 0 );
			break;
		}

		case VIEW_LAST_USERS:
		{
			view_last_users( cmd );
			break;
		}

		case SAVE_REPORT:
		{
			save_report( cmd );
			break;
		}

		case VIEW_REPORT:
		{
			view_report( cmd );
			break;
		}

		case SEARCH_NAME:
		{
			search_name( cmd );
			break;
		}

		case VIEW_LAST_REPORTS:
		{
			view_last_reports( cmd );
			break;
		}

		default:
			error_cmd();
			break;
	};

	return TRUE;
};

int view_last_reports( struct command* cmd )
{
	system( "ls -lt reports/" );

	return TRUE;
};

int search_name( struct command* cmd )
{
	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;
	
	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;
	
	BYTE AccessTokenSize = payload[ payload_off ];
	payload_off++;

	BYTE* AccessToken = (BYTE*) calloc( AccessTokenSize, sizeof( BYTE ) );	
	memcpy( AccessToken, payload + payload_off, AccessTokenSize );
	payload_off += AccessTokenSize;

	// get report name size and report name data
	BYTE SearchedNameSize = payload[ payload_off ];
	payload_off++;

	BYTE* SearchedName = (BYTE*) calloc( SearchedNameSize, sizeof( BYTE ) );
	memcpy( SearchedName, payload + payload_off, SearchedNameSize );
	payload_off += SearchedNameSize;

	sanitize( Username );
	sanitize( AccessToken );

	if ( !login( cmd, 1 ) )
	{
		return FALSE;
	}
	
	if ( SearchedNameSize <= 4 )
	{
		send_error( ERROR_SEARCHED_NAME );
		return FALSE;
	}

	struct UsersList* users_list = NULL;

	users_list = search_user( SearchedName );

	if ( users_list == NULL )
	{
		send_error( ERR_SEARCH_USER_LIST_INIT );
		return FALSE;
	}

	if ( users_list->current_id == 0 )
	{
		send_error( ERR_SEARCH_NO_USERS );
		return FALSE;
	}

	send_user_list( users_list );

	free( Username );
	free( AccessToken );

	return TRUE;
};

int view_report( struct command* cmd )
{

	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;
	
	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;
	
	BYTE AccessTokenSize = payload[ payload_off ];
	payload_off++;
	payload_off += AccessTokenSize;

	// get report name size and report name data
	BYTE ReportNameSize = payload[ payload_off ];
	payload_off++;

	BYTE* ReportName = (BYTE*) calloc( ReportNameSize, sizeof( BYTE ) );
	memcpy( ReportName, payload + payload_off, ReportNameSize );
	payload_off += ReportNameSize;

	// get report type size and report type data
	BYTE ReportTypeSize = payload[ payload_off ];
	payload_off++;

	BYTE* ReportType = (BYTE*) calloc( ReportTypeSize, sizeof( BYTE ) );
	memcpy( ReportType, payload + payload_off, ReportTypeSize );
	payload_off += ReportTypeSize;
			
	BYTE* ReportFilePath = (BYTE*) calloc( MAX_FILE_PATH, sizeof( BYTE ) );
	
	sanitize( ReportName );
	sanitize( Username );

	strcpy( ReportFilePath, "reports/" );
	strcat( ReportFilePath, Username );
	strcat( ReportFilePath, "_" );
	strcat( ReportFilePath, ReportName );

	// chek if report is private	
	if ( !memcmp( ReportType, "private", ReportTypeSize ) )
	{
		strcat( ReportFilePath, ".priv" );

		int size = 0;
		BYTE* record_data = read_file( ReportFilePath, &size );

		if ( record_data == NULL )
		{
			free( ReportFilePath );
			free( ReportName );
			free( ReportType );
			free( Username );

			send_error( FILE_NOT_FOUND );
			return FALSE;
		}

		if ( !login( cmd, 1 ) )
		{
			send_raw_record( record_data, size );
		}
		else
		{
			BYTE ReportPasswordSize = payload[ payload_off ];
			payload_off++;

			if ( ReportPasswordSize <= 4 || ReportPasswordSize > 16 )
			{
				send_error( ERR_SAVE_REP_PASSWORD_SIZE );
				
				free( ReportFilePath );
				free( ReportName );
				free( ReportType );
				free( record_data );

				return FALSE;
			}

			BYTE* ReportPassword = (BYTE*) calloc( ReportPasswordSize, 
				sizeof( BYTE ) );

			memcpy( ReportPassword, payload + payload_off, ReportPasswordSize );
			payload_off += ReportPasswordSize;


			BYTE* decrypted_record = encode_report_data( 
				cmd,
				record_data,
				size,
				ReportPassword,
				ReportPasswordSize
			);

			send_raw_record( decrypted_record, size );
		}
	}
	else
	{
		if ( !login( cmd, 1 ) )
		{
			free( ReportFilePath );
			free( ReportName );
			free( ReportType );
			free( Username );

			return FALSE;
		}

		strcat( ReportFilePath, ".pub" );

		int size = 0;
		BYTE* record_data = read_file( ReportFilePath, &size );

		if ( record_data == NULL )
		{
			free( ReportFilePath );
			free( ReportName );
			free( ReportType );
			free( Username );

			send_error( FILE_NOT_FOUND );
			return FALSE;
		}

		send_raw_record( record_data, size );
	}

	free( ReportFilePath );
	free( ReportName );
	free( ReportType );
	free( Username );

	return TRUE;

};

int save_report( struct command* cmd )
{
	if ( !login( cmd, 1 ) )
	{
		return FALSE;
	}

	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;
	
	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;

	
	BYTE AccessTokenSize = payload[ payload_off ];
	payload_off++;
	payload_off += AccessTokenSize;

	// get report name size and report name data
	BYTE ReportNameSize = payload[ payload_off ];
	payload_off++;

	BYTE* ReportName = (BYTE*) calloc( ReportNameSize, sizeof( BYTE ) );
	memcpy( ReportName, payload + payload_off, ReportNameSize );
	payload_off += ReportNameSize;

	// get report type size and report type data
	BYTE ReportTypeSize = payload[ payload_off ];
	payload_off++;

	BYTE* ReportType = (BYTE*) calloc( ReportTypeSize, sizeof( BYTE ) );
	memcpy( ReportType, payload + payload_off, ReportTypeSize );
	payload_off += ReportTypeSize;
		
	// get report data size and report data value
	BYTE ReportDataSize = payload[ payload_off ];
	payload_off++;

	BYTE* ReportData = (BYTE*) calloc( ReportDataSize, sizeof( BYTE ) );
	memcpy( ReportData, payload + payload_off, ReportDataSize );
	payload_off += ReportDataSize;
	
	// chek if report is private
	BYTE* ReportFilePath = (BYTE*) calloc( MAX_FILE_PATH, sizeof( BYTE ) );
	
	sanitize( ReportName );
	sanitize( Username );

	strcpy( ReportFilePath, "reports/" );
	strcat( ReportFilePath, Username );
	strcat( ReportFilePath, "_" );
	strcat( ReportFilePath, ReportName );

	int saved = 0;

	if ( !memcmp( ReportType, "private", ReportTypeSize ) )
	{

		BYTE ReportPasswordSize = payload[ payload_off ];
		payload_off++;

		if ( ReportPasswordSize <= 4 || ReportPasswordSize > 16 )
		{
			send_error( ERR_SAVE_REP_PASSWORD_SIZE );
			
			free( ReportFilePath );
			free( ReportName );
			free( ReportType );
			free( ReportData );

			return FALSE;
		}

		BYTE* ReportPassword = (BYTE*) calloc( ReportPasswordSize, 
			sizeof( BYTE ) );

		memcpy( ReportPassword, payload + payload_off, ReportPasswordSize );
		payload_off += ReportPasswordSize;
		

		BYTE* encrypted_report_data = encode_report_data( 
			cmd, 
			ReportData, 
			ReportDataSize,
			ReportPassword, 
			ReportPasswordSize
		);

		strcat( ReportFilePath, ".priv" );

		saved = save_data_in_file( 
			ReportFilePath, 
			encrypted_report_data, 
			ReportDataSize 
		);

		free( encrypted_report_data );
		free( ReportPassword );
	}
	else
	{
		strcat( ReportFilePath, ".pub" );

		saved = save_data_in_file( 
			ReportFilePath, 
			ReportData, 
			ReportDataSize 
		);
	}

	free( ReportFilePath );
	free( ReportName );
	free( ReportType );
	free( ReportData );

	if ( !saved )
	{	
		send_error( REPORT_NOT_SAVED );
		free( Username );
		return FALSE;
	}
	else
	{
		send_valid_report_save( Username );
	}

	free( Username );

	return TRUE;
};

int login( struct command* cmd, int no_out )
{
	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;

	if ( UsernameSize >= cmd->size )
	{
		send_error( ERR_REG_USERNAME_SIZE );
		return FALSE;
	}

	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;

	BYTE AccessTokenSize = payload[ payload_off ];
	payload_off++;

	if ( AccessTokenSize >= cmd->size || 
		AccessTokenSize >= ( cmd->size - UsernameSize ) 
		)
	{
		send_error( ERR_REG_STATUS_SIZE );
		free( Username );
		return FALSE;
	}

	// check if user exist in database
	if ( !check_user_exist( Username ) )
	{
		send_error( ERR_LOGIN_USER_NOT_EXIST );
		free( Username );

		return FALSE;
	}

	BYTE* AccessToken = (BYTE*) calloc( AccessTokenSize, sizeof( BYTE ) );	
	memcpy( AccessToken, payload + payload_off, AccessTokenSize );
	payload_off += AccessTokenSize;

	if ( !check_user_with_token( Username, AccessToken ) )
	{
		send_error( ERR_LOGIN_TOKEN_INVALID );
		free( Username );
		free( AccessToken );

		return FALSE;
	}

	if ( !no_out )
		send_valid_login( Username, AccessToken );

	free( Username );
	free( AccessToken );

	return TRUE;
};

int view_last_users( struct command* cmd )
{
	//puts( "[+] Debug! view_last_users start!" );

	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE TokenSize = payload[ payload_off ];
	payload_off++;

	if ( TokenSize >= cmd->size )
	{
		send_error( ERR_VIEW_TOKEN_SIZE );
		return FALSE;
	}

	BYTE* Token = (BYTE*) calloc( TokenSize, sizeof( BYTE ) );
	memcpy( Token, payload + payload_off, TokenSize );

	struct UsersList* users_list = NULL;

	if ( !memcmp( Token, LOCALHOST_CONNECTION_TOKEN, TokenSize ) )
	{
		users_list = get_last_users(); // db function
	}
	else
	{
		send_error( ERR_VIEW_INVALID_TOKEN_VALUE );
		free( Token );
		return FALSE;
	}

	if ( users_list == NULL )
	{
		send_error( ERR_VIEW_USER_LIST_INIT );
		return FALSE;
	}

	if ( users_list->current_id == 0 )
	{
		send_error( ERR_VIEW_NO_USERS );
		return FALSE;
	}

	// send total count of users
	send_user_list( users_list );

	return TRUE;
};

int registration( struct command* cmd )
{
	BYTE* payload = cmd->payload;
	int payload_off = 0;

	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;

	if ( UsernameSize >= cmd->size )
	{
		send_error( ERR_REG_USERNAME_SIZE );
		return FALSE;
	}

	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;

	BYTE StatusSize = payload[ payload_off ];
	payload_off++;

	if ( StatusSize >= cmd->size || 
		StatusSize >= ( cmd->size - UsernameSize ) 
		)
	{
		send_error( ERR_REG_STATUS_SIZE );
		free( Username );
		return FALSE;
	}

	// check if user exist in database
	if ( check_user_exist( Username ) )
	{
		send_error( ERR_REG_USER_EXIST );
		free( Username );
		return FALSE;
	}

	BYTE* Status = (BYTE*) calloc( StatusSize, sizeof( BYTE ) );	
	memcpy( Status, payload + payload_off, StatusSize );
	payload_off += StatusSize;

	if ( payload_off == cmd->size )
	{
		send_error( ERR_REG_EMPTY_TOKEN );

		free( Username );
		free( Status );

		return FALSE;
	}

	int TokenSize = payload[ payload_off ];
	
	if ( TokenSize > ( cmd->size - payload_off ) ) 
	{
		send_error( ERR_REG_INVALID_TOKEN_SIZE );

		free( Username );
		free( Status );
		
		return FALSE;
	}

	payload_off++; 

	BYTE* Token = (BYTE*) calloc( TokenSize, sizeof( BYTE ) );
	memcpy( Token, payload + payload_off, TokenSize );

	BYTE* access_token = NULL;

	if ( !memcmp( Token, LOCALHOST_CONNECTION_TOKEN, TokenSize ) )
	{
		access_token = generate_access_token( Username, Status );
	}
	else
	{
		send_error( ERR_REG_INVALID_TOKEN_VALUE );

		free( Username );
		free( Status );
		free( Token );

		return FALSE;
	}

	if ( access_token == NULL )
	{
		send_error( ERR_REG_ACCESS_TOKEN_GEN );

		free( Username );
		free( Status );
		free( Token );

		return FALSE;
	}

	if ( add_new_user_to_db( Username, Status, access_token ) )
	{
		send_valid_reg( Username, 
			Status, 
			access_token, 
			UsernameSize, 
			StatusSize
		);
	}
	else
	{
		free( Username );
		free( Status );
		free( Token );
		free( access_token );

		send_error( ERR_REG_ADD_USER_ERROR );
		return FALSE;
	}

	free( Username );
	free( Status );
	free( Token );
	free( access_token );

	return TRUE;
};

BYTE* generate_access_token( BYTE* Username, BYTE* Status )
{
	BYTE* access_token = (BYTE*) calloc( ACCESS_TOKEN_SIZE, sizeof( BYTE ) );

	int seed = 0x0;

	for ( int i = 0; i < strlen( Status ); i++ )
	{
		for ( int j = 0; j < strlen( Username ); j++ )
		{
			seed += i + j;
			seed += Status[ i ];
			seed += Username[ j ];
		}
	}
	
	srand( seed );

	for ( int i = 0; i < strlen( Username ); i++ )
	{
		seed -= i;
		seed -= Username[ i ];
		seed -= ( rand() % 10 );
	}

	srand( seed );

	for ( int i = 0; i < ACCESS_TOKEN_SIZE; i++ )
	{
		access_token[ i ] = token_alph[ rand() % 62 ]; 
	}

	return access_token;
};

void send_valid_login( BYTE* Username, BYTE* AccessToken )
{	
	int UsernameSize = strlen( Username );
	int AccessTokenSize = strlen( AccessToken );

	int packet_size = UsernameSize + 1; 
	packet_size += AccessTokenSize + 1; 
	
	packet_size += 2; // size of login cmd
	packet_size++; // login flag

	BYTE* packet = (BYTE*) calloc( packet_size + 4, sizeof( BYTE ) );
	int packet_offset = 0;

	memcpy( packet, (void*)&packet_size, sizeof( DWORD ) );
	packet_offset += 4;

	BYTE* loging_cmd = pack_16_le( LOGIN_COMMAND );

	memcpy( packet + packet_offset, loging_cmd, sizeof( WORD ) );
	free( loging_cmd );
	packet_offset += 2;

	packet[ packet_offset++ ] = (BYTE) UsernameSize;
	memcpy( packet + packet_offset, Username, UsernameSize );
	packet_offset += UsernameSize;

	packet[ packet_offset++ ] = (BYTE) AccessTokenSize;
	memcpy( packet + packet_offset, AccessToken, AccessTokenSize );
	packet_offset += AccessTokenSize;

	packet[ packet_offset ] = (BYTE) 1;

	write( 1, packet, packet_size + 4 );
};

void send_user_entry( BYTE* UserEntry, int UserEntrySize )
{
	write( 1, UserEntry, UserEntrySize );
}

void send_valid_reg( 
	BYTE* Username, 
	BYTE* Status, 
	BYTE* access_token,
	int UsernameSize,
	int StatusSize
)
{
	// + 1 is byte to set size of field
	int packet_size = UsernameSize + 1; 
	packet_size += StatusSize + 1; 
	packet_size += ACCESS_TOKEN_SIZE + 1; 
	packet_size++;

	packet_size += 2; // size of REGISTER_COMMAND

	BYTE* packet = (BYTE*) calloc( packet_size + 4, sizeof( BYTE ) );
	int packet_offset = 0;

	memcpy( packet, (void*)&packet_size, sizeof( DWORD ) );
	packet_offset += 4;

	BYTE* reg_cmd = pack_16_le( REGISTER_COMMAND );

	memcpy( packet + packet_offset, reg_cmd, sizeof( WORD ) );
	free( reg_cmd );
	packet_offset += 2;

	packet[ packet_offset++ ] = (BYTE) UsernameSize;
	memcpy( packet + packet_offset, Username, UsernameSize );
	packet_offset += UsernameSize;

	packet[ packet_offset++ ] = (BYTE) StatusSize;
	memcpy( packet + packet_offset, Status, StatusSize );
	packet_offset += StatusSize;

	packet[ packet_offset++ ] = (BYTE) ACCESS_TOKEN_SIZE;
	memcpy( packet + packet_offset, access_token, ACCESS_TOKEN_SIZE );

	write( 1, packet, packet_size + 4 );
};

void error_cmd( void )
{
	send_error( ERR_CMD_ID );
};

BYTE* encode_report_data( 
	struct command* cmd, 
	BYTE* data, 
	int data_size,
	BYTE* ReportPassword,
	int ReportPasswordSize 
)
{
	BYTE* payload = cmd->payload;
	int payload_off = 0;

	// get Username 
	BYTE UsernameSize = payload[ payload_off ];
	payload_off++;

	BYTE* Username = (BYTE*) calloc( UsernameSize, sizeof( BYTE ) );	
	memcpy( Username, payload + payload_off, UsernameSize );
	payload_off += UsernameSize;

	// get Access token
	BYTE AccessTokenSize = payload[ payload_off ];
	payload_off++;

	BYTE* AccessToken = (BYTE*) calloc( AccessTokenSize, sizeof( BYTE ) );	
	memcpy( AccessToken, payload + payload_off, AccessTokenSize );
	payload_off += AccessTokenSize;

	// generate gamma
	int UsernameBytesSum = 0;

	for ( int i = 0; i < UsernameSize; i++ )
		UsernameBytesSum += (int) Username[ i ];

	int AccessTokenBytesSum = 0;

	for ( int i = 0; i < AccessTokenSize; i++ )
		AccessTokenBytesSum += (int) AccessToken[ i ];

	int ReportPasswordBytesSum = 0;

	for ( int i = 0; i < ReportPasswordSize; i++ )
		ReportPasswordBytesSum += (int) ReportPassword[ i ];

	int seed = UsernameBytesSum + AccessTokenBytesSum + data_size;
	seed += ReportPasswordBytesSum;

	BYTE* encode_key = generate_key( seed );
	BYTE* encrypted_data = (BYTE*) calloc( data_size, sizeof( BYTE ) );

	cipher( encode_key, data, encrypted_data, data_size );

	free( encode_key );
	free( Username );
	free( AccessToken );

	return encrypted_data;
};

BYTE* generate_key( int seed )
{
	srand( seed );

	BYTE* key = (BYTE*) calloc( ENCODE_KEY_SIZE, sizeof( BYTE ) );

	for ( int i = 0; i < ENCODE_KEY_SIZE; i++ )
	{
		key[ i ] = (BYTE) ( rand() % 255 );
	}

	return key;
};

void send_valid_report_save( BYTE* Username )
{
	int UsernameSize = strlen( Username );

	int packet_size = UsernameSize + 1; 
	
	packet_size += 2; // size of report cmd
	packet_size++; // report flag

	BYTE* packet = (BYTE*) calloc( packet_size + 4, sizeof( BYTE ) );
	int packet_offset = 0;

	memcpy( packet, (void*)&packet_size, sizeof( DWORD ) );
	packet_offset += 4;

	BYTE* save_report_cmd = pack_16_le( SAVE_REPORT );

	memcpy( packet + packet_offset, save_report_cmd, sizeof( WORD ) );
	free( save_report_cmd );
	packet_offset += 2;

	packet[ packet_offset++ ] = (BYTE) UsernameSize;
	memcpy( packet + packet_offset, Username, UsernameSize );
	packet_offset += UsernameSize;

	packet[ packet_offset ] = (BYTE) 1;

	write( 1, packet, packet_size + 4 );
};

void send_raw_record( BYTE* record_data, int record_size )
{
	int packet_size = record_size;
	packet_size += 2; // view record cmd size
	packet_size++;

	BYTE* packet = (BYTE*) calloc( packet_size + 4, sizeof( BYTE ) );
	int packet_offset = 0;

	memcpy( packet + packet_offset, (void*)&packet_size, sizeof( DWORD ) );
	packet_offset += sizeof( DWORD );

	BYTE* view_report_cmd = pack_16_le( VIEW_REPORT );
	
	memcpy( packet + packet_offset, view_report_cmd, sizeof( WORD ) );
	free( view_report_cmd );
	packet_offset += sizeof( WORD );

	packet[ packet_offset++ ] = (BYTE) record_size;
	memcpy( packet + packet_offset, record_data, record_size );
	packet_offset += record_size;

	write( 1, packet, packet_size + 4 );
};


void send_user_list( struct UsersList* users_list )
{
	write( 1, &(users_list->current_id), sizeof( int ) );

	for ( int i = 0; i < users_list->current_id; i++ )
	{
		int uname_size = strlen( users_list->list[ i ].Username );
		int atoken_size = strlen( users_list->list[ i ].AccessToken );
		int status_size = strlen( users_list->list[ i ].Status );

		int user_entry_size = 0;

		user_entry_size += sizeof( int ); // id 
		user_entry_size += sizeof( int ); // is_admin
		
		user_entry_size += sizeof( BYTE ); // username size
		user_entry_size += uname_size;
		
		user_entry_size += sizeof( BYTE ); // access token size
		user_entry_size += atoken_size;

		user_entry_size += sizeof( BYTE ); // status size
		user_entry_size += status_size;

		int user_entry_offset = 0;
		BYTE* UserEntry = (BYTE*) calloc( user_entry_size + 4, 
			sizeof( BYTE ) );

		// set total packet size 
		memcpy( UserEntry + user_entry_offset, 
			&user_entry_size, sizeof( int ) );

		user_entry_offset += sizeof( int );

		// set id value
		memcpy( UserEntry + user_entry_offset, 
			&( users_list->list[ i ].id ), sizeof( int ) );

		user_entry_offset += sizeof( int );

		// set username size 
		UserEntry[ user_entry_offset++ ] = (BYTE) uname_size;
		
		// set username
		memcpy( UserEntry + user_entry_offset, 
			users_list->list[ i ].Username, uname_size );
		
		user_entry_offset += uname_size;

		// set atoken_size
		UserEntry[ user_entry_offset++ ] = (BYTE) atoken_size;

		// set atoken
		memcpy( UserEntry + user_entry_offset, 
			users_list->list[ i ].AccessToken, atoken_size );

		user_entry_offset += atoken_size;

		// set status size
		UserEntry[ user_entry_offset++ ] = (BYTE) status_size;

		// set status
		memcpy( UserEntry + user_entry_offset,
			users_list->list[ i ].Status, status_size );

		user_entry_offset += status_size;

		// set is_admin
		memcpy( UserEntry + user_entry_offset, 
			&( users_list->list[ i ].is_admin ), sizeof( int ) );

		send_user_entry( UserEntry, user_entry_size + 4 );

		free( UserEntry );
	}

	for ( int i = 0; i < users_list->current_id; i++ )
	{
		free( users_list->list[ i ].Username );
		free( users_list->list[ i ].AccessToken );
		free( users_list->list[ i ].Status );
	}

	free( users_list );
}

