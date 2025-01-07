#include "stdafx.h"
#include "MY_STUDENT.h"
#include "cli_mess.h"
#include "stack.h"


const char* sfields_text[] = { "Mathematics", "Biology", "Computer Science" };

/**
 * @brief Tworzy nowego studenta.
 *
 * @param surname Nazwisko studenta.
 * @param birth_year Rok urodzenia studenta.
 * @param sfield Kierunek studiów studenta (typ wyliczeniowy StudyField).
 *
 * @return WskaŸnik na nowo utworzon¹ strukturê MyStudent.
 *         W przypadku b³êdu alokacji pamiêci zg³asza CLI_MESS_ALLOC_ERROR.
 *
 * @details Funkcja alokuje pamiêæ dla nowego obiektu typu MyStudent oraz dla jego pola surname.
 *          Nastêpnie kopiuje nazwisko, ustawia rok urodzenia i kierunek studiów, a na koñcu
 *          zwraca wskaŸnik do utworzonego obiektu.
 *          W przypadku b³êdu alokacji pamiêci dla studenta lub nazwiska, funkcja zg³asza b³¹d.
 */
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

/**
 * @brief Zwalnia pamiêæ zajmowan¹ przez obiekt MyStudent.
 *
 * @param student WskaŸnik na strukturê MyStudent, która ma zostaæ zwolniona.
 *
 * @details Funkcja zwalnia pamiêæ przydzielon¹ dla pola `surname` oraz samej struktury `student`.
 */
void free_student(MyStudent* student)
{
	free(student->surname);
	free(student);
}

/**
 * @brief Zapisuje dane studenta do pliku.
 *
 * @param pdat WskaŸnik na dane studenta, które maj¹ zostaæ zapisane.
 * @param filename Nazwa pliku, do którego dane maj¹ zostaæ zapisane.
 *
 * @details Funkcja zapisuje dane studenta (nazwisko, rok urodzenia, kierunek studiów) do pliku w trybie binarnym.
 *          Przed zapisaniem nazwiska, zapisuje jego d³ugoœæ, a nastêpnie samo nazwisko oraz pozosta³e dane.
 *          W przypadku b³êdów podczas zapisu (b³¹d otwarcia pliku, b³¹d zapisu), funkcja zg³asza odpowiednie komunikaty.
 *          Funkcja zapisuje dane w trybie "ab" (dodawanie do pliku), wiêc nie nadpisuje istniej¹cych danych.
 */
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

    //Zapisywanie dynamicznej zmiennej
    size_t surname_len = strlen(student->surname) + 1;
    if (fwrite(&surname_len, sizeof(size_t), 1, file) != 1) {
        throw_cli_mess(CLI_MESS_FILE_WRITE_ERROR);
        fclose(file);
        return;
    }

    //Zapisywanie nazwiska
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

/**
 * @brief Odczytuje dane studentów z pliku i dodaje je na stos.
 *
 * @param stack WskaŸnik na stos, do którego maj¹ zostaæ dodane odczytane dane studentów.
 * @param filename Nazwa pliku, z którego dane maj¹ zostaæ odczytane.
 *
 * @details Funkcja odczytuje dane studentów zapisane w pliku binarnym. Dla ka¿dego studenta odczytywana jest d³ugoœæ nazwiska,
 *          nazwisko, rok urodzenia oraz kierunek studiów. Nastêpnie alokowana jest pamiêæ dla struktury MyStudent,
 *          dane s¹ przypisywane do odpowiednich pól, a student jest dodawany na stos. Funkcja koñczy dzia³anie, gdy
 *          osi¹gnie koniec pliku lub napotka b³¹d odczytu/zapisu. W przypadku b³êdów (brak pliku, b³¹d alokacji pamiêci,
 *          b³¹d odczytu), funkcja zg³asza odpowiednie komunikaty o b³êdach.
 */
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
        // Odczyt d³ugoœci nazwiska
        size_t surname_len;
        if (fread(&surname_len, sizeof(size_t), 1, file) != 1) {
            break; // Koniec pliku
        }

        // Alokowanie pamiêci dla nazwiska
        char* surname = (char*)malloc(surname_len);
        if (!surname) {
            throw_cli_mess(CLI_MESS_ALLOC_ERROR);
            fclose(file);
            return;
        }

        // Odczyt nazwiska
        if (fread(surname, sizeof(char), surname_len, file) != surname_len) {
            free(surname);
            throw_cli_mess(CLI_MESS_FILE_READ_ERROR);
            fclose(file);
            return;
        }

        int birth_year;
        StudyField sfield;
        if (fread(&birth_year, sizeof(int), 1, file) != 1 ||
            fread(&sfield, sizeof(StudyField), 1, file) != 1) {
            free(surname);
            throw_cli_mess(CLI_MESS_FILE_READ_ERROR);
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

void find_and_print_students_by_criteria(Stack* stack, const char* surname, int birth_year, StudyField sfield, const char* filename) {
    // Jeœli stos jest pusty odczyt danych 
    if (!stack || !filename) {
        throw_cli_mess(CLI_MESS_INVALID_ARGUMENT);
        return;
    }

    if (stack->top == NULL) {
        // Odczyt studenta z pliku tylko raz jeœli stos jest pusty
        read_student_from_file(stack, filename);
    }

    printf("Wyniki wyszukiwania:\n");

    StackItem* current = stack->top;
    while (current) {
        MyStudent* student = (MyStudent*)current->data;

        int match = 1;

        // Sprawdzamy nazwisko, jeœli zosta³o podane
        if (surname && strlen(surname) > 0 && strcmp(student->surname, surname) != 0) {
            match = 0;
        }

        // Sprawdzamy rok urodzenia, jeœli zosta³ podany
        if (birth_year != -1 && student->birth_year != birth_year) {
            match = 0;
        }

        // Sprawdzamy kierunek, jeœli zosta³ podany
        if (sfield != -1 && student->sfield != sfield) {
            match = 0;
        }

        // Jeœli student pasuje do kryteriów, wyœwietlamy dane
        if (match) {
            printf("Nazwisko: %s, Rok Urodzenia: %d, Kierunek: %s\n",
                student->surname, student->birth_year, sfields_text[student->sfield]);
        }

        current = current->next;
    }
}


/**
 * @brief Wyœwietla wszystkich studentów znajduj¹cych siê na stosie.
 *
 * @param stack WskaŸnik na stos, który zawiera dane studentów.
 *
 * @details Funkcja iteruje przez wszystkie elementy stosu i wyœwietla dane ka¿dego studenta (nazwisko, rok urodzenia, kierunek studiów).
 *          Jeœli stos jest niezainicjalizowany lub pusty, zg³aszane s¹ odpowiednie komunikaty o b³êdach.
 *          Dla ka¿dego studenta wyœwietlany jest numer porz¹dkowy oraz jego dane.
 */
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