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
 * @param sfield Kierunek studi�w studenta (typ wyliczeniowy StudyField).
 *
 * @return Wska�nik na nowo utworzon� struktur� MyStudent.
 *         W przypadku b��du alokacji pami�ci zg�asza CLI_MESS_ALLOC_ERROR.
 *
 * @details Funkcja alokuje pami�� dla nowego obiektu typu MyStudent oraz dla jego pola surname.
 *          Nast�pnie kopiuje nazwisko, ustawia rok urodzenia i kierunek studi�w, a na ko�cu
 *          zwraca wska�nik do utworzonego obiektu.
 *          W przypadku b��du alokacji pami�ci dla studenta lub nazwiska, funkcja zg�asza b��d.
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
 * @brief Zwalnia pami�� zajmowan� przez obiekt MyStudent.
 *
 * @param student Wska�nik na struktur� MyStudent, kt�ra ma zosta� zwolniona.
 *
 * @details Funkcja zwalnia pami�� przydzielon� dla pola `surname` oraz samej struktury `student`.
 */
void free_student(MyStudent* student)
{
	free(student->surname);
	free(student);
}

/**
 * @brief Zapisuje dane studenta do pliku.
 *
 * @param pdat Wska�nik na dane studenta, kt�re maj� zosta� zapisane.
 * @param filename Nazwa pliku, do kt�rego dane maj� zosta� zapisane.
 *
 * @details Funkcja zapisuje dane studenta (nazwisko, rok urodzenia, kierunek studi�w) do pliku w trybie binarnym.
 *          Przed zapisaniem nazwiska, zapisuje jego d�ugo��, a nast�pnie samo nazwisko oraz pozosta�e dane.
 *          W przypadku b��d�w podczas zapisu (b��d otwarcia pliku, b��d zapisu), funkcja zg�asza odpowiednie komunikaty.
 *          Funkcja zapisuje dane w trybie "ab" (dodawanie do pliku), wi�c nie nadpisuje istniej�cych danych.
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
 * @brief Odczytuje dane student�w z pliku i dodaje je na stos.
 *
 * @param stack Wska�nik na stos, do kt�rego maj� zosta� dodane odczytane dane student�w.
 * @param filename Nazwa pliku, z kt�rego dane maj� zosta� odczytane.
 *
 * @details Funkcja odczytuje dane student�w zapisane w pliku binarnym. Dla ka�dego studenta odczytywana jest d�ugo�� nazwiska,
 *          nazwisko, rok urodzenia oraz kierunek studi�w. Nast�pnie alokowana jest pami�� dla struktury MyStudent,
 *          dane s� przypisywane do odpowiednich p�l, a student jest dodawany na stos. Funkcja ko�czy dzia�anie, gdy
 *          osi�gnie koniec pliku lub napotka b��d odczytu/zapisu. W przypadku b��d�w (brak pliku, b��d alokacji pami�ci,
 *          b��d odczytu), funkcja zg�asza odpowiednie komunikaty o b��dach.
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
        // Odczyt d�ugo�ci nazwiska
        size_t surname_len;
        if (fread(&surname_len, sizeof(size_t), 1, file) != 1) {
            break; // Koniec pliku
        }

        // Alokowanie pami�ci dla nazwiska
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
    // Je�li stos jest pusty odczyt danych 
    if (!stack || !filename) {
        throw_cli_mess(CLI_MESS_INVALID_ARGUMENT);
        return;
    }

    if (stack->top == NULL) {
        // Odczyt studenta z pliku tylko raz je�li stos jest pusty
        read_student_from_file(stack, filename);
    }

    printf("Wyniki wyszukiwania:\n");

    StackItem* current = stack->top;
    while (current) {
        MyStudent* student = (MyStudent*)current->data;

        int match = 1;

        // Sprawdzamy nazwisko, je�li zosta�o podane
        if (surname && strlen(surname) > 0 && strcmp(student->surname, surname) != 0) {
            match = 0;
        }

        // Sprawdzamy rok urodzenia, je�li zosta� podany
        if (birth_year != -1 && student->birth_year != birth_year) {
            match = 0;
        }

        // Sprawdzamy kierunek, je�li zosta� podany
        if (sfield != -1 && student->sfield != sfield) {
            match = 0;
        }

        // Je�li student pasuje do kryteri�w, wy�wietlamy dane
        if (match) {
            printf("Nazwisko: %s, Rok Urodzenia: %d, Kierunek: %s\n",
                student->surname, student->birth_year, sfields_text[student->sfield]);
        }

        current = current->next;
    }
}


/**
 * @brief Wy�wietla wszystkich student�w znajduj�cych si� na stosie.
 *
 * @param stack Wska�nik na stos, kt�ry zawiera dane student�w.
 *
 * @details Funkcja iteruje przez wszystkie elementy stosu i wy�wietla dane ka�dego studenta (nazwisko, rok urodzenia, kierunek studi�w).
 *          Je�li stos jest niezainicjalizowany lub pusty, zg�aszane s� odpowiednie komunikaty o b��dach.
 *          Dla ka�dego studenta wy�wietlany jest numer porz�dkowy oraz jego dane.
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