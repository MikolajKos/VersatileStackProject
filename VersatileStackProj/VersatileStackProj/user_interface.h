#include "stdafx.h"
#include "stack.h"

#pragma once

void UserMenu();
void interf_free_stack(Stack* stack);
void interf_push(Stack* stack);
void* interf_pop(Stack* stack);
void interf_read_stack(Stack* stack);
