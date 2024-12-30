#pragma once

#include "stdafx.h"

enum CLIENT_MESSAGES {
	CLI_MESS_ALLOC_ERROR,
	CLI_MESS_WARN,
	CLI_MESS_STACK_EMPTY,
	TOTAL_MESSAGES
};

void throw_cli_mess(enum CLIENT_MESSAGES mess);