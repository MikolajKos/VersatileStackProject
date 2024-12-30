#include "stdafx.h"
#include <string.h>

#pragma once

typedef struct StackItem {
	void* data;
	struct StackItem* next;
}StackItem;

typedef struct Stack {
	StackItem* top;
}Stack;

Stack* initialize_stack();
void free_stack(Stack* stack);
void push(Stack* stack, void* data);
void* pop(Stack* stack);