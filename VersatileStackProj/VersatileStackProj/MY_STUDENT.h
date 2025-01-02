#include "stdafx.h"
#include "cli_mess.h"
#include <string.h>

#pragma once

typedef enum {
	MT,
	BG,
	CS
} StudyField;

const char* sfields_text[];

typedef struct {
	char* surname;
	int birth_year;
	StudyField sfield;
} MyStudent;

MyStudent* create_student(const char* surname, int birth_year, StudyField sfield);
void free_student(MyStudent* student);
void save_student_to_file(void** pdat, const char* filename);
void load_student_from_file(void** pdat, const char* filename);


