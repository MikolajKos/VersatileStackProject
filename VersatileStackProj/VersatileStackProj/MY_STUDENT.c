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

    FILE* file = fopen(filename, "ab");
    if (!file) {
        throw_cli_mess(CLI_MESS_FILE_ERROR);
        return;
    }

    MyStudent* student = (MyStudent*)*pdat;

    //Saving dynamic data size
    size_t surname_len = strlen(student->surname) + 1;
    if (fwrite(&surname_len, sizeof(size_t), 1, file) != 1) {
        throw_cli_mess(CLI_MESS_FILE_WRITE_ERROR);
        fclose(file);
        return;
    }

    //Saving surname
    if (fwrite(student->surname, sizeof(char), surname_len, file) != surname_len) {
        throw_cli_mess(CLI_MESS_FILE_WRITE_ERROR);
        fclose(file);
        return;
    }

    if (fwrite(&student->birth_year, sizeof(int), 1, file) != 1 ||
        fwrite(&student->sfield, sizeof(StudyField), 1, file) != 1) {
        throw_cli_mess(CLI_MESS_FILE_WRITE_ERROR);
        fclose(file);
        return;
    }

    fclose(file);
}

void read_student_from_file(Stack* stack, const char* filename) {
    if (!stack || !filename) {
        throw_cli_mess(CLI_MESS_INVALID_ARGUMENT);
        return;
    }

    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw_cli_mess(CLI_MESS_FILE_ERROR);
        return;
    }

    while (1) {
        // Read surname length
        size_t surname_len;
        if (fread(&surname_len, sizeof(size_t), 1, file) != 1) {
            break; // Koniec pliku
        }

        // Surname memory allocation
        char* surname = (char*)malloc(surname_len);
        if (!surname) {
            throw_cli_mess(CLI_MESS_ALLOC_ERROR);
            fclose(file);
            return;
        }

        // Read surname
        if (fread(surname, sizeof(char), surname_len, file) != surname_len) {
            free(surname);
            //throw_cli_mess(CLI_MESS_FILE_READ_ERROR);
            fclose(file);
            return;
        }

        int birth_year;
        StudyField sfield;
        if (fread(&birth_year, sizeof(int), 1, file) != 1 ||
            fread(&sfield, sizeof(StudyField), 1, file) != 1) {
            free(surname);
            //throw_cli_mess(CLI_MESS_FILE_READ_ERROR);
            fclose(file);
            return;
        }

        MyStudent* student = (MyStudent*)malloc(sizeof(MyStudent));
        if (!student) {
            free(surname);
            throw_cli_mess(CLI_MESS_ALLOC_ERROR);
            fclose(file);
            return;
        }

        student->surname = surname;
        student->birth_year = birth_year;
        student->sfield = sfield;

        // Dodajemy studenta na stos
        push(stack, student);
    }

    fclose(file);
}

void print_all_students(Stack* stack) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_UNINITIALIZED_STACK);
        return;
    }

    StackItem* current = stack->top;
    if (!current) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return;
    }

    printf("Lista studentow:\n");
    int index = 1;

    while (current) {
        MyStudent* student = (MyStudent*)current->data;
        if (student) {
            printf("%d. Nazwisko: %s, Rok Urodzenia: %d, Kierunek: %s\n",
                index,
                student->surname,
                student->birth_year,
                sfields_text[student->sfield]);
        }
        current = current->next;
        index++;
    }
}