#include "stdafx.h"
#include "cli_mess.h"
#include "stack.h"

/**
 * @brief Inicjalizuje nowy stos.
 *
 * @return WskaŸnik na nowo utworzon¹ strukturê stosu. W przypadku b³êdu alokacji zg³asza CLI_MESS_ALLOC_ERROR.
 */
Stack* initialize_stack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (!stack)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	stack->top = NULL;
	return;
}

/**
 * @brief Zwalnia pamiêæ zajmowan¹ przez stos i jego elementy.
 *
 * @param stack Podwójny wskaŸnik na strukturê stosu. Pozwala ustawiæ oryginalny wskaŸnik na NULL po zwolnieniu pamiêci.
 *
 * @details Funkcja iteracyjnie zwalnia pamiêæ zajmowan¹ przez elementy stosu i dane w nich przechowywane.
 *          Nastêpnie zwalnia pamiêæ samej struktury stosu i ustawia wskaŸnik na NULL, aby unikn¹æ ponownego u¿ycia.
 *          Je¿eli wskaŸnik jest NULL lub wskazuje na NULL, zg³aszany jest b³¹d CLI_MESS_UNINITIALIZED_STACK.
 *
 * @return Funkcja nie zwraca wartoœci.
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
	*stack = NULL; // Ustawienie wskaŸnika na NULL, aby unikn¹æ podwójnego zwolnienia.
}

/**
 * @brief Dodaje nowy element na szczyt stosu.
 *
 * @param stack WskaŸnik na strukturê stosu.
 * @param data WskaŸnik na dane, które maj¹ zostaæ dodane do stosu.
 *
 * @note W przypadku b³êdu alokacji zg³asza CLI_MESS_ALLOC_ERROR.
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
 * @brief Usuwa i zwraca element znajduj¹cy siê na szczycie stosu.
 *
 * @param stack WskaŸnik na strukturê stosu, z którego ma zostaæ usuniêty element.
 *
 * @return WskaŸnik na dane przechowywane w usuniêtym elemencie.
 *         Jeœli stos jest pusty, zwraca NULL i zg³asza CLI_MESS_STACK_EMPTY.
 *
 * @details Funkcja usuwa element znajduj¹cy siê na szczycie stosu, zwalnia pamiêæ zajmowan¹ przez ten element,
 *          aktualizuje wskaŸnik top stosu oraz zwraca wskaŸnik na dane usuniêtego elementu.
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
 * @brief Zwraca wszystkie dane przechowywane w stosie jako tablicê wskaŸników.
 *
 * @param stack WskaŸnik na strukturê stosu, z którego dane maj¹ zostaæ odczytane.
 * @param count WskaŸnik na zmienn¹, w której zostanie zapisana liczba elementów w stosie.
 *
 * @return WskaŸnik na tablicê wskaŸników przechowuj¹c¹ dane ze stosu.
 *         Zwraca NULL, jeœli stos jest pusty lub wyst¹pi³ b³¹d alokacji pamiêci.
 *
 * @details Funkcja iteracyjnie zlicza elementy stosu, alokuje pamiêæ na tablicê wskaŸników
 *          i wype³nia j¹ danymi z kolejnych elementów stosu.
 *          W przypadku pustego stosu zg³asza CLI_MESS_STACK_EMPTY.
 *          W przypadku b³êdu alokacji pamiêci zg³asza CLI_MESS_ALLOC_ERROR.
 */
void** peek_all(Stack* stack, int* count) {
	*count = 0;

	if (!stack) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	}

	// Zliczanie elementów stosu
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