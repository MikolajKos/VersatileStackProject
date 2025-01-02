#include "stdafx.h"
#include "MY_STUDENT.h"
#include "cli_mess.h"
#include "stack.h"


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

	strcpy_s(student->surname, strlen(surname) + 1, surname);

	student->birth_year = birth_year;
	student->sfield = sfield;

	return student;
}

void free_student(MyStudent* student)
{
	free(student->surname);
	free(student);
}

void save_student_to_file(void** pdat, const char* filename) {
    if (!pdat || !filename) {
        throw_cli_mess(CLI_MESS_INVALID_ARGUMENT);
        return;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        throw_cli_mess(CLI_MESS_FILE_ERROR);
        return;
    }

    MyStudent* student = (MyStudent*)*pdat;
    if (fwrite(student, sizeof(MyStudent), 1, file) != 1) {
        throw_cli_mess(CLI_MESS_FILE_WRITE_ERROR);
        fclose(file);
        return;
    }

    fclose(file);
}

void load_student_from_file(void** ptr, const char* filename);