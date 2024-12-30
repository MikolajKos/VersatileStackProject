#include "stdafx.h"
#include "MY_STUDENT.h"

const char* sfields_text[] = { "Mathematics", "Biology", "Computer Science" };

MyStudent* create_student(const char* surname, int birth_year, StudyField sfield) {
	MyStudent* student = (MyStudent*)malloc(sizeof(MyStudent));
	if (!student)
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);

	//Surname memory allocation
	student->surname = (char*)malloc(strlen(surname) + 1);
	if (!student->surname) {
		throw_cli_mess(CLI_MESS_ALLOC_ERROR);
		free(student);
	}

	strcpy_s(student->surname, strlen(student->surname), surname);

	student->birth_year = birth_year;
	student->sfield = sfield;

	return student;
}

void free_student(MyStudent* student)
{
	free(student->surname);
	free(student);
}