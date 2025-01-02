#include "stdafx.h"
#include "stack.h"

#pragma once

void UserMenu();
void interf_free_stack(Stack* stack);
void interf_push(Stack* stack, int option);
void interf_pop(Stack* stack, int option);
void interf_peek_all(Stack* stack, int option);
