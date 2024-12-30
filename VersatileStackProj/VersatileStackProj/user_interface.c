#include "stdafx.h"
#include "user_interface.h"

void UserMenu(Stack* stack){
	int option;

	do {
		printf_s("Wybierz dzialanie:\n");
		printf_s("  1 - zwolnij pamiec stosu\n  2 - dodaj nowy element\n  3 - pobierz pierwszy element\n  4 - znajdz element\n  5 - zapisz na dysku\n  6 - odczytaj z dysku\n  0 - Wyjdz\n\n");
		scanf_s("%d", &option);

		switch (option) {
			case 1:
				interf_free_stack(stack);
			case 2:
				/*interf_push()*/
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

void interf_push(Stack* stack, void* data) {
	/*printf_s("Wybierz da");*/
	
	push(stack, data);
}

void* interf_pop(Stack* stack) {
	pop(stack);
}