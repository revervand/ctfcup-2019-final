#pragma once

#include "main.h"

struct command
{
	WORD id;
	DWORD size;
	BYTE* payload;
};

int parse_packet( BYTE* );