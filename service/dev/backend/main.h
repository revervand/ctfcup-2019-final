#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/*
	packet format

	int size;// +0, size of all data after this field; // + 0
	BYTE command; // + 4, command to proceed
	BYTE* command_payload // +5 
*/

// some consts
#define DEFAULT_ALARM_TIME 120
#define TRUE 1 
#define FALSE 0 
#define MAX_PACKET_SIZE 4096
#define ERR_MSG_SIZE 8

typedef unsigned int DWORD;
typedef unsigned short int WORD;
typedef unsigned char BYTE;

struct User 
{
	BYTE* name;
	BYTE* password;
	BYTE status;
	WORD id;
	BYTE* session_token;
};

void setup( void );
int parse_packet( BYTE* );
void init_user( void );

// ecs_secret_token