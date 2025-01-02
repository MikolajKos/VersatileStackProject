#include "stdafx.h"
#include "cli_mess.h"
#include <string.h>

static char* text_messages[] = {
	"E Blad alokowania pamieci",
	"W Nieznany blad",
	"W Stos jest pusty",
	"E Stos jest pusty",
	"W Program obecnie nie jest przystosowany do obslugi tego obiektu",
	"W Wybrano nieistniejaca opcje, wybierz jeszcze raz"
};

void throw_cli_mess(enum CLIENT_MESSAGES mess) {
	puts(text_messages[mess] + 2);

	switch (text_messages[mess][0]){
		case 'E':
			system("pause");
		default:
			break;
	}
	return;
}