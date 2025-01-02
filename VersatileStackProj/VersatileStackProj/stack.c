#include "stdafx.h"
#include "cli_mess.h"
#include "stack.h"

Stack* initialize_stack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (!stack)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	stack->top = NULL;
	return;
}

void free_stack(Stack* stack) {
	if (stack == NULL) {
		throw_cli_mess(CLI_MESS_UNINITIALIZED_STACK);
		return NULL;
	}

	StackItem* item = stack->top;
	while (item) {
		stack->top = item->next;
		free(item->data);
		free(item);
		item = stack->top;
	}
	free(stack);
}

void push(Stack* stack, void* data) {
	StackItem* item = (StackItem*)malloc(sizeof(StackItem));
	if (!item)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	item->data = data;
	item->next = stack->top;
	stack->top = item;
}

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

void** peek_all(Stack* stack, int* count) {
	*count = 0;

	if (!stack) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
	}

	// Liczba elementów w stosie
	StackItem* current = stack->top;
	while (current) {
		(*count)++;
		current = current->next;
	}

	// Sprawdzenie czy stos nie jest pusty
	if (*count == 0) {
		throw_cli_mess(CLI_MESS_STACK_EMPTY);
		return NULL;
	}

	// Alokacja tablicy wskaŸników
	void** data_array = (void**)malloc((*count) * sizeof(void*));
	if (!data_array) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
		return NULL;
	}

	// Wype³nianie tablicy danymi
	current = stack->top;
	for (int i = 0; i < *count; i++) {
		data_array[i] = current->data;
		current = current->next;
	}

	return data_array;
}