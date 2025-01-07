#include "stdafx.h"
#include "stack.h"

#pragma once

void UserMenu();
void interf_free_stack(Stack** stack);
Stack* interf_initialize_stack();
void interf_push(Stack* stack, int option);
void interf_pop(Stack* stack, int option);
void interf_peek_all(Stack* stack, int option);
void interf_save_to_file(Stack* stack, const char* filename, int option);
void interf_load_from_file(Stack* stack, const char* filename, int option);
void interf_find_by_criteria(const char* filename, int option);