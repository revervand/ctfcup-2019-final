#pragma once

#include "main.h"
#include "parser.h"
#include "utils.h"
#include "err.h"
#include "db.h"
#include "rc4.h"

#define ACCESS_TOKEN_SIZE 16
#define ENCODE_KEY_SIZE 32
#define MAX_FILE_PATH 1024

// Server commands
#define REGISTER_COMMAND  0x7E12
#define LOGIN_COMMAND     0x7E13
#define VIEW_LAST_USERS   0x7E14
#define SAVE_REPORT 	  0x7E15
#define VIEW_REPORT		  0x7E16
#define SEARCH_NAME 	  0x7E17
#define VIEW_LAST_REPORTS 0x7E18

 
int send_error( int _errno );
int run_command( struct command* );
int login( struct command*, int );
int registration( struct command* );
int view_last_users( struct command* );
int save_report( struct command* );
int view_report( struct command* );
int search_name( struct command* );
int view_last_reports( struct command* );

BYTE* encode_report_data( struct command*, BYTE*, int, BYTE*, int );
BYTE* generate_key( int );

void send_valid_reg( 
	BYTE* Username, 
	BYTE* Status, 
	BYTE* access_token,
	int UsernameSize,
	int StatusSize
);

void send_user_entry( BYTE*, int );
void send_valid_login( BYTE*, BYTE* );
void send_valid_report_save( BYTE* );
void send_raw_record( BYTE*, int );
void send_user_list( struct UsersList* );

//int login( struct command* );

BYTE* generate_access_token( BYTE* Username, BYTE* Status );

static BYTE LOCALHOST_CONNECTION_TOKEN[] = "ecs_secret_token\0";
static BYTE token_alph[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789";

// errors func
void error_cmd( void );
