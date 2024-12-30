#include "stdafx.h"
#include "user_interface.h"
#include "MY_STUDENT.h"

void UserMenu(Stack* stack){
	int option;

	do {
		printf_s("Wybierz dzialanie:\n");
		printf_s("  1 - zwolnij pamiec stosu\n  2 - dodaj nowy element\n  3 - pobierz pierwszy element\n  4 - znajdz element\n  5 - zapisz na dysku\n  6 - odczytaj z dysku\n  0 - Wyjdz\n\n");
		scanf_s("%d", &option);

		switch (option) {
			case 1:
				interf_free_stack(stack);
				break;
			case 2:
				interf_push(stack);
				break;
			default:
				break;
		}
	} while (option != 0);

}

Stack* interf_initialize_stack() { 
	initialize_stack(); 
}

void interf_free_stack(Stack* stack) { 
	free_stack(stack); 
}

void interf_push(Stack* stack) {
	int option;
	
	do {
		printf_s("Wybierz rodzaj danych:\n");
		printf_s("  1 - student\n  2 - samochod\n  0 - Wroc do menu\n");
		scanf_s("%d", &option);

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
			printf_s("Program obecnie nie jest przystosowany do obslugi tego obiektu\n");
		}
		default:
			break;
		}
	} while (option != 0);
}

void* interf_pop(Stack* stack) {
	pop(stack);
}