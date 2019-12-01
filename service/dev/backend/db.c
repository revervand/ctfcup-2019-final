#include "db.h"

sqlite3 * open_database( void )
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open( DB_PATH, &db );

	if ( rc ) 
	{
		return NULL;
	} 

	return db;
};

int close_database( sqlite3 * db )
{
	sqlite3_close( db );

	return 1;
};

int check_db( void )
{
	char *zErrMsg = 0;
	int rc;
	char* data = (char*) calloc( BUF_SIZE, sizeof( char ) );

	if ( access( DB_PATH, F_OK ) != -1 ) 
	{
		sqlite3 *db = open_database();

		if( db == NULL )
			return 0;

    	sqlite3_stmt *stmt;
		const char *sql = "SELECT 1 FROM sqlite_master where type='table' and name=?";
		char* tbl_name = "users";

		int rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );

		if ( rc != SQLITE_OK ) 
		{
			printf( "error: %s", sqlite3_errmsg( db ) );
			return 0;
		}

		sqlite3_bind_text( stmt, 1, tbl_name, -1, SQLITE_TRANSIENT );

		rc = sqlite3_step( stmt );
		int found;
		
		if ( rc == SQLITE_ROW )
			found = 1;

		else if ( rc == SQLITE_DONE )
			found = 0;
		
		else 
		{
			printf( "error: %", sqlite3_errmsg( db ) );
			sqlite3_finalize( stmt );

			return 0;
		}

		sqlite3_finalize( stmt );
		close_database( db );

		if ( !found )
			return 0;
		else
			return 1;
	} 
	else 
	{
    	return 0;
	}

	return 0;
};

int init_db( void )
{
	sqlite3* db = open_database();
	
	char *zErrMsg = 0;
	int rc;

	if ( db == NULL )
	{
		return 0;
	}

	const char* query = "CREATE TABLE users( " \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ," \
		"USERNAME CHAR(32) NOT NULL," \
		"ACCESS_TOKEN CHAR(32) NOT NULL," \
		"STATUS CHAR(32) NOT NULL," \
		"IS_ADMIN INT NOT NULL);";

	rc = sqlite3_exec( db, query, NULL, 0, &zErrMsg );
   
	if( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );

		return 0;
	}
	
	close_database( db );
	
	db = open_database();

	const char* sql = "INSERT INTO users " \
		"(ID,USERNAME,ACCESS_TOKEN,STATUS,IS_ADMIN) " \
		"VALUES (NULL, 'revker', 'kerrev', 'root', 1);";

	rc = sqlite3_exec( db, sql, NULL, 0, &zErrMsg );

	if ( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );
		return 0;
	} 
	else 
	{
		return 1;
	}

	close_database( db );

	return 0;
};

int check_user_exist( char* Username )
{
	char *zErrMsg = 0;
	sqlite3 *db = open_database();

	if( db == NULL )
		return 0;

	sqlite3_stmt *stmt;
	const char *sql = "SELECT 1 FROM users where USERNAME=?";

	int rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );

	if ( rc != SQLITE_OK ) 
	{
		printf( "error: %s", sqlite3_errmsg( db ) );
		return 0;
	}

	sqlite3_bind_text( stmt, 1, Username, -1, SQLITE_TRANSIENT );

	rc = sqlite3_step( stmt );
	int found;
	
	if ( rc == SQLITE_ROW )
		found = 1;

	else if ( rc == SQLITE_DONE )
		found = 0;
	
	else 
	{
		printf( "error: %", sqlite3_errmsg( db ) );
		sqlite3_finalize( stmt );

		return 0;
	}

	sqlite3_finalize( stmt );
	close_database( db );

	if ( !found )
		return 0;
	else
		return 1;
};

int add_new_user_to_db( char* Username, char* Status, char* Token )
{
	sqlite3* db = open_database();

	char *zErrMsg = 0;
	int rc;

	if ( db == NULL )
	{
		return 0;
	}

	const char* base_query = "INSERT INTO users " \
	"(ID,USERNAME,ACCESS_TOKEN,STATUS,IS_ADMIN) " \
	"VALUES (NULL,'";

	char* query = (char*) calloc( BUF_SIZE, sizeof( BYTE ) );

	strcpy( query, base_query );
	strcat( query, Username );
	strcat( query, "', " );
	strcat( query, "'" );
	strcat( query, Token );
	strcat( query, "', " );
	strcat( query, "'" );
	strcat( query, Status );
	strcat( query, "', " );
	strcat( query, "0);\x00" );

	rc = sqlite3_exec( db, query, NULL, 0, &zErrMsg );

	close_database( db );

	if( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );

		return 0;
	} 
	else 
	{
		return 1;
	}
	
	return 1;	
};

int callback_get_users( void* StructPtr, 
	int argc, 
	char** argv, 
	char** azColName
)
{
	struct UsersList* list = (struct UsersList*) StructPtr; 
	int current_id = list->current_id;

	if ( argc == 5 )
	{
		list->list[ current_id ].id 		  = atoi( argv[ 0 ] );
		list->list[ current_id ].is_admin 	  = atoi( argv[ 4 ] );
	
		list->list[ current_id ].Username 	  = (BYTE*) malloc( strlen( argv[ 1 ] ) );
		list->list[ current_id ].AccessToken  = (BYTE*) malloc( strlen( argv[ 2 ] ) );
		list->list[ current_id ].Status 	  = (BYTE*) malloc( strlen( argv[ 3 ] ) );
		
		strcpy( list->list[ current_id ].Username, argv[ 1 ] );
		strcpy( list->list[ current_id ].AccessToken, argv[ 2 ] );
		strcpy( list->list[ current_id ].Status, argv[ 3 ] );
	}

	list->current_id++;

	return 0;
};

struct UsersList* get_last_users( void )
{
	struct UsersList* last_users = (struct UsersList*)
		malloc( sizeof( struct UsersList ) );

	last_users->current_id = 0;

	sqlite3* db = open_database();

	char *zErrMsg = 0;
	int rc;

	if ( db == NULL )
	{
		return 0;
	}

	const char* query = "SELECT * FROM users ORDER BY ID DESC LIMIT 50";

	rc = sqlite3_exec( db, query, callback_get_users, last_users, &zErrMsg );

	if ( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );

		return 0;
	}

	// for ( int i = 0; i < last_users->current_id; i++ )
	// {
	// 	printf( "Id: %d\n", last_users->list[i].id ); 
	// 	printf( "Username: %s\n", last_users->list[i].Username );
	// }

	// for ( int i = 0; i < last_users->current_id; i++ )
	// {
	// 	free( last_users->list[ i ].Username );
	// 	free( last_users->list[ i ].AccessToken );
	// 	free( last_users->list[ i ].Status );
	// }

	// free( last_users );
	close_database( db );

	return last_users;
};

int callback_user_with_token( void* flag, 
	int argc, 
	char** argv, 
	char** azColName
)
{
	BYTE* _flag = (BYTE*) flag;

	if ( argc == 5 )
		*_flag = (BYTE) 1;
	else
		*_flag = (BYTE) 0;

	return 0;
}

int check_user_with_token( BYTE* Username, BYTE* AccessToken )
{
	sqlite3* db = open_database();

	char *zErrMsg = 0;
	int rc;

	if ( db == NULL )
	{
		return 0;
	}

	sanitize( Username );
	sanitize( AccessToken );

	const char* base_query = "SELECT * FROM users WHERE USERNAME='";

	char* sql_query = (char*) malloc( strlen( base_query ) 
		+ strlen( Username ) 
		+ strlen( AccessToken ) 
		+ 0x128 );

	strcpy( sql_query, base_query );
	strcat( sql_query, Username );
	strcat( sql_query, "' AND ACCESS_TOKEN='" );
	strcat( sql_query, AccessToken );
	strcat( sql_query, "'\0" );


	BYTE* find = (BYTE*) malloc( sizeof( BYTE ) );
	*find = 0;

	rc = sqlite3_exec( db, sql_query, callback_user_with_token, find, &zErrMsg );

	if ( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );

		return 0;
	}

	if ( *find )
		return 1;
	else
		return 0;

	return 1;
};

void sanitize( BYTE* buffer )
{
	for ( int i = 0; i < strlen( buffer ); i++ )
	{
		if ( ( buffer[ i ] >= 'A' && buffer[ i ] <= 'Z' ) || 
			( buffer[ i ] >= 'a' && buffer[ i ] <= 'z' ) || 
			( buffer[ i ] >= '0' && buffer[ i ] <= '9' ) || 
			( buffer[ i ] == '_')
		)
		{
			;
		}
		else
		{
			buffer[ i ] = '\0'; 
		}
	}
};

struct UsersList* search_user( BYTE* Username )
{
	struct UsersList* last_users = (struct UsersList*)
		malloc( sizeof( struct UsersList ) );

	last_users->current_id = 0;

	sqlite3* db = open_database();

	char *zErrMsg = 0;
	int rc;

	if ( db == NULL )
	{
		return 0;
	}

	const char* query = "SELECT * FROM users WHERE USERNAME LIKE '%"; // %' ORDER BY ID DESC LIMIT 50";

	char* sql_query = (char*) calloc( BUF_SIZE, sizeof( BYTE ) );

	strcpy( sql_query, query );
	strcat( sql_query, Username );
	strcat( sql_query, "%' ORDER BY ID DESC LIMIT 50" ); 

	rc = sqlite3_exec( db, sql_query, callback_get_users, last_users, &zErrMsg );

	if ( rc != SQLITE_OK )
	{
		fprintf( stderr, "SQL error: %s\n", zErrMsg );
		sqlite3_free( zErrMsg );

		return 0;
	}
	
	close_database( db );

	return last_users;
};