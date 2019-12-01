#pragma once

#include "main.h"

#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DB_PATH "users.db"
#define BUF_SIZE 4096

sqlite3 * open_database( void );
int check_db( void );
int init_db( void );
int close_database( sqlite3 * );
int add_new_user_to_db( char* , char* , char* );
int check_user_exist( char* );
int check_user_with_token( BYTE* , BYTE* );

struct UsersList* search_user( BYTE* );

struct UsersList* get_last_users( void );
int callback_get_users( void*, int, char**, char** );
int callback_user_with_token( void*, int, char**, char** );

void sanitize( BYTE* );

struct UserEntry
{
	int id;
	BYTE* Username;
	BYTE* AccessToken;
	BYTE* Status;
	int is_admin;
};

struct UsersList
{
	struct UserEntry list[ 50 ];
	int current_id;
};