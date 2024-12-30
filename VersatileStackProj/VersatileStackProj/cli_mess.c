#include "stdafx.h"
#include "cli_mess.h"
#include <string.h>

static char* text_messages[] = {
	"E Memory allocarion error",
	"W Unknown warning",
	"W Stack is empty"
};

void throw_cli_mess(enum CLIENT_MESSAGES mess) {
	puts(text_messages[mess] + 2);

	if (text_messages[mess][0] == 'E')
	{
		system("pause");
	}
	return;
}