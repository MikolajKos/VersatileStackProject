#include "stdafx.h"
#include "cli_mess.h"
#include "stack.h"
#include "MY_STUDENT.h"
#include "user_interface.h"

int main() {
	Stack* stack = initialize_stack();
	UserMenu(stack);

	//push(stack, create_student("Piotr", 2004, CS));
	//push(stack, create_student("Andrzej", 2002, MT));
	//push(stack, create_student("Krzysztof", 1999, BG));
	//push(stack, create_student("Mateusz", 2003, CS));

	MyStudent* result;
	
	while ((result = (MyStudent *)pop(stack)) != NULL)
	{
		printf_s("Surname: %s, Birth Year: %d, Field: %s \n",
			result->surname,
			result->birth_year,
			sfields_text[result->sfield]);

		printf_s("\n");
	}
}