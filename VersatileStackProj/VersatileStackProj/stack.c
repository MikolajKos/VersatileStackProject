#include "stdafx.h"
#include "cli_mess.h"
#include "stack.h"

/**
 * @brief Inicjalizuje nowy stos.
 *
 * @return Wska�nik na nowo utworzon� struktur� stosu. W przypadku b��du alokacji zg�asza CLI_MESS_ALLOC_ERROR.
 */
Stack* initialize_stack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (!stack)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	stack->top = NULL;
	return;
}

/**
 * @brief Zwalnia pami�� zajmowan� przez stos i jego elementy.
 *
 * @param stack Podw�jny wska�nik na struktur� stosu. Pozwala ustawi� oryginalny wska�nik na NULL po zwolnieniu pami�ci.
 *
 * @details Funkcja iteracyjnie zwalnia pami�� zajmowan� przez elementy stosu i dane w nich przechowywane.
 *          Nast�pnie zwalnia pami�� samej struktury stosu i ustawia wska�nik na NULL, aby unikn�� ponownego u�ycia.
 *          Je�eli wska�nik jest NULL lub wskazuje na NULL, zg�aszany jest b��d CLI_MESS_UNINITIALIZED_STACK.
 *
 * @return Funkcja nie zwraca warto�ci.
 *
 * @note Zabezpieczona przed ponownym zwalnianiem tego samego stosu.
 */
void free_stack(Stack** stack) {
	if (stack == NULL || *stack == NULL) {
		throw_cli_mess(CLI_MESS_UNINITIALIZED_STACK);
		return;
	}

	StackItem* item = (*stack)->top;
	while (item) {
		(*stack)->top = item->next;
		free(item->data); // Zwalnianie danych.
		free(item);       // Zwalnianie elementu stosu.
		item = (*stack)->top;
	}

	free(*stack); // Zwalnianie struktury stosu.
	*stack = NULL; // Ustawienie wska�nika na NULL, aby unikn�� podw�jnego zwolnienia.
}

/**
 * @brief Dodaje nowy element na szczyt stosu.
 *
 * @param stack Wska�nik na struktur� stosu.
 * @param data Wska�nik na dane, kt�re maj� zosta� dodane do stosu.
 *
 * @note W przypadku b��du alokacji zg�asza CLI_MESS_ALLOC_ERROR.
 */
void push(Stack* stack, void* data) {
	StackItem* item = (StackItem*)malloc(sizeof(StackItem));
	if (!item)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	item->data = data;
	item->next = stack->top;
	stack->top = item;
}

/**
 * @brief Usuwa i zwraca element znajduj�cy si� na szczycie stosu.
 *
 * @param stack Wska�nik na struktur� stosu, z kt�rego ma zosta� usuni�ty element.
 *
 * @return Wska�nik na dane przechowywane w usuni�tym elemencie.
 *         Je�li stos jest pusty, zwraca NULL i zg�asza CLI_MESS_STACK_EMPTY.
 *
 * @details Funkcja usuwa element znajduj�cy si� na szczycie stosu, zwalnia pami�� zajmowan� przez ten element,
 *          aktualizuje wska�nik top stosu oraz zwraca wska�nik na dane usuni�tego elementu.
 */
void* pop(Stack* stack) {
	if (!stack->top) {
		throw_cli_mess(CLI_MESS_STACK_EMPTY);
		return NULL;
	}
	StackItem* item = stack->top;
	void* data = item->data;
	stack->top = item->next;
	free(item);
	return data;
}

/**
 * @brief Zwraca wszystkie dane przechowywane w stosie jako tablic� wska�nik�w.
 *
 * @param stack Wska�nik na struktur� stosu, z kt�rego dane maj� zosta� odczytane.
 * @param count Wska�nik na zmienn�, w kt�rej zostanie zapisana liczba element�w w stosie.
 *
 * @return Wska�nik na tablic� wska�nik�w przechowuj�c� dane ze stosu.
 *         Zwraca NULL, je�li stos jest pusty lub wyst�pi� b��d alokacji pami�ci.
 *
 * @details Funkcja iteracyjnie zlicza elementy stosu, alokuje pami�� na tablic� wska�nik�w
 *          i wype�nia j� danymi z kolejnych element�w stosu.
 *          W przypadku pustego stosu zg�asza CLI_MESS_STACK_EMPTY.
 *          W przypadku b��du alokacji pami�ci zg�asza CLI_MESS_ALLOC_ERROR.
 */
void** peek_all(Stack* stack, int* count) {
	*count = 0;

	if (!stack) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	}

	// Zliczanie element�w stosu
	StackItem* current = stack->top;
	while (current) {
		(*count)++;
		current = current->next;
	}

	if (*count == 0) {
		throw_cli_mess(CLI_MESS_STACK_EMPTY);
		return NULL;
	}

	void** data_array = (void**)malloc((*count) * sizeof(void*));
	if (!data_array) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
		return NULL;
	}

	current = stack->top;
	for (int i = 0; i < *count; i++) {
		data_array[i] = current->data;
		current = current->next;
	}

	return data_array;
}