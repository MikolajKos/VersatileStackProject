#include "stdafx.h"
#include "stack.h"

#pragma once

void UserMenu();
Stack* interf_initialize_stack();
void interf_free_stack(Stack* stack);
void interf_push(Stack* stack, void* data);
void* interf_pop(Stack* stack);
