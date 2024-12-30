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
	StackItem* item = stack->top;
	while (item) {
		stack->top = item->next;
		free(item);
		item = stack->top;
	}
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
	return data;
}