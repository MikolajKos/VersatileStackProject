#include "stdafx.h"
#include "user_interface.h"
#include "MY_STUDENT.h"
#include "cli_mess.h"
#include "data.h"

void UserMenu(){
	int option, type_option;

	Stack* stack = initialize_stack();

	do{
		printf_s("Wybierz rodzaj danych:\n");
		printf_s("  1 - student\n  2 - samochod\n");
		scanf_s("%d", &type_option);

		if (type_option != 1 && type_option != 2)
			throw_cli_mess(CLI_MESS_OPTION_WARN);
		else
			break;
	} while (true);

	do {
		printf_s("Wybierz dzialanie:\n");
		printf_s("  1 - zwolnij pamiec stosu\n  2 - dodaj nowy element\n  3 - pobierz pierwszy element\n  4 - znajdz element\n  5 - wypisz wszystkie elementy stosu\n  6 - zapisz na dysku\n  7 - odczytaj z dysku\n  8 - Zainicjalizuj stos\n  0 - Wyjdz\n\n");
		scanf_s("%d", &option);

		switch (option) {
			case 1:
				interf_free_stack(stack);
				break;
			case 2:
				interf_push(stack, type_option);
				break;
			case 3:
				interf_pop(stack, type_option);
				break;
			case 5:
				interf_peek_all(stack, type_option);
				break;
			case 6:
				interf_save_to_file(stack, "stackdata.bin", type_option);
				break;
			case 7:
				interf_load_from_file(stack, "stackdata.bin", type_option);
				break;
			case 8:
				stack = initialize_stack();
				break;
			default:
				break;
		}
	} while (option != 0);


}

void interf_free_stack(Stack* stack) { 
	free_stack(stack); 
}

void interf_push(Stack* stack, int option) {
	switch (option) {
		case 1: {
			char surname[50];
			int birth_year;
			int field_od_study;

			printf_s("Podaj nazwisko:\n");
			scanf_s("%s", surname, sizeof(surname));
			printf("Podaj rok urodzenia:\n");
			scanf_s("%d", &birth_year);																   
			printf("Podaj kierunek studiow:\n");													   
			scanf_s("%d", &field_od_study);															   
																									   
			MyStudent* student = create_student(surname, birth_year, field_od_study);				   																						   
			push(stack, student);																	   
																									   
			break;																					   
		}																							   
		case 2: {																					   
			throw_cli_mess(CLI_MESS_WRONG_TYPE);
		}																							   
		default:																					   
			break;																					   
	}																			   
}																									   
																									   
void interf_pop(Stack* stack, int option) {																	   
	switch (option){
		case 1: {
			MyStudent* result;
			if (!(result = (void*)pop(stack))) {
				break;
			}
			else {
				printf_s("Surname: %s, Birth Year: %d, Field: %s \n",
					result->surname,
					result->birth_year,
					sfields_text[result->sfield]);
			}
			break;
		}
		case 2: {
			throw_cli_mess(CLI_MESS_WRONG_TYPE);
			break;
		}
		default:
			break;
	}																			   
}																									   

void interf_peek_all(Stack* stack, int option) {
	int count = 0;

	switch (option) {
	case 1: {
		MyStudent** result = peek_all(stack, &count);

		for (int i = 0; i < count; i++) {
			printf_s("Surname: %s, Birth Year: %d, Field: %s \n",
				result[i]->surname,
				result[i]->birth_year,
				sfields_text[result[i]->sfield]);
		}

		break;
	}
	case 2: {
		throw_cli_mess(CLI_MESS_WRONG_TYPE);
		break;
	}
	default:
		break;
	}
}

void interf_save_to_file(Stack* stack, const char* filename, int option) {
	switch (option){
		case 1: {
			save_student_to_file(stack->top, filename);
			break;
		}
		case 2: {
			throw_cli_mess(CLI_MESS_WRONG_TYPE);
			break;
		}
		default:
			break;
	}
}

void interf_load_from_file(Stack* stack, const char* filename, int option) {
	switch (option) {
		case 1: {
			load_stack_from_file(stack, filename, sizeof(MyStudent));

			while (stack->top) {
				MyStudent* result = (MyStudent*)pop(stack);

				printf_s("Surname: %s, Birth Year: %d, Field: %s \n",
					result->surname,
					result->birth_year,
					sfields_text[result->sfield]);
				free(result);
			}

			break;
		}
		case 2: {
			throw_cli_mess(CLI_MESS_WRONG_TYPE);
			break;
		}
		default:
			break;
	}
}

Stack* interf_initialize_stack() {
	
}
