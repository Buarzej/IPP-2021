/** @file
  Implementacja kalkulatora wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

/** Wymagane do poprawnego działania funkcji getline(). */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "stack.h"
#include "utilities.h"
#include "poly_parser.h"
#include "calc_commands.h"

/** Znak rozpoczynający linię z komentarzem. */
#define COMMENT_CHAR '#'

/** Początkowy rozmiar stosu. */
#define STACK_STARTING_SIZE 8

/**
 * Bezpieczne wywołanie funkcji getline.
 * @param[in] buffer : bufor
 * @param[in] bufferSize : rozmiar bufora
 * @return długość wczytanej linii
 */
static ssize_t safeGetline(char **buffer, size_t *bufferSize) {
    errno = 0;
    ssize_t getlineSize = getline(buffer, bufferSize, stdin);
    if (errno == ENOMEM) exit(1);
    return getlineSize;
}

/**
 * Wypisuje błąd `WRONG COMMAND` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorWrongCommand(int lineIndex) {
    fprintf(stderr, "ERROR %d WRONG COMMAND\n", lineIndex);
}

/**
 * Wypisuje błąd `STACK UNDERFLOW` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorStackUnderflow(int lineIndex) {
    fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", lineIndex);
}

/**
 * Wypisuje błąd `WRONG POLY` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorWrongPoly(int lineIndex) {
    fprintf(stderr, "ERROR %d WRONG POLY\n", lineIndex);
}

/**
 * Wypisuje błąd `DEG BY WRONG VARIABLE` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorDegByWrongVariable(int lineIndex) {
    fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", lineIndex);
}

/**
 * Wypisuje błąd `AT WRONG VALUE` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorAtWrongValue(int lineIndex) {
    fprintf(stderr, "ERROR %d AT WRONG VALUE\n", lineIndex);
}

/**
 * Wypisuje błąd `COMPOSE WRONG PARAMETER` na standardowe wyjście błędów.
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ErrorComposeWrongParameter(int lineIndex) {
    fprintf(stderr, "ERROR %d COMPOSE WRONG PARAMETER\n", lineIndex);
}

/**
 * Parsuje linię tekstu zawierającą komendę kalkulatora
 * przyjmującą argument, wywołując odpowiednie polecenie.
 * @param[in] string : wczytana linia tekstu
 * @param[in] stack : stos
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ParseArgumentCommand(char *string, Stack *stack, int lineIndex) {
    if (strncmp(string, "DEG_BY", 6) == 0) {
        if (string[6] != ' ' || !isdigit((int) string[7])) {
            if (!isspace((int) string[6]) && string[6] != '\0')
                ErrorWrongCommand(lineIndex);
            else
                ErrorDegByWrongVariable(lineIndex);
        } else {
            char *remaining;
            errno = 0;
            size_t argument = strtoul(&string[7], &remaining, 10);

            if (remaining[0] != '\0' || errno == ERANGE)
                ErrorDegByWrongVariable(lineIndex);
            else if (!CalcDegBy(stack, argument))
                ErrorStackUnderflow(lineIndex);
        }
    } else if (strncmp(string, "AT", 2) == 0) {
        if (string[2] != ' ' ||
            (!isdigit((int) string[3]) && string[3] != '-')) {
            if (!isspace((int) string[2]) && string[2] != '\0')
                ErrorWrongCommand(lineIndex);
            else
                ErrorAtWrongValue(lineIndex);
        } else {
            char *remaining;
            errno = 0;
            poly_coeff_t argument = strtol(&string[3], &remaining, 10);

            if (remaining[0] != '\0' || errno == ERANGE)
                ErrorAtWrongValue(lineIndex);
            else if (!CalcAt(stack, argument))
                ErrorStackUnderflow(lineIndex);
        }
    } else if (strncmp(string, "COMPOSE", 7) == 0) {
        if (string[7] != ' ' || !isdigit((int) string[8])) {
            if (!isspace((int) string[7]) && string[7] != '\0')
                ErrorWrongCommand(lineIndex);
            else
                ErrorComposeWrongParameter(lineIndex);
        } else {
            char *remaining;
            errno = 0;
            size_t argument = strtoul(&string[8], &remaining, 10);

            if (remaining[0] != '\0' || errno == ERANGE)
                ErrorComposeWrongParameter(lineIndex);
            else if (!CalcCompose(stack, argument))
                ErrorStackUnderflow(lineIndex);
        }
    } else {
        ErrorWrongCommand(lineIndex);
    }
}

/**
 * Parsuje linię tekstu zawierającą komendę kalkulatora,
 * wywołując odpowiednie polecenie.
 * @param[in] string : wczytana linia tekstu
 * @param[in] stack : stos
 * @param[in] lineIndex : indeks wczytanej linii
 */
static void ParseCommand(char *string, Stack *stack, int lineIndex) {
    string = strtok(string, "\n");
    if (strcmp(string, "ZERO") == 0) {
        CalcZero(stack);
    } else if (strcmp(string, "IS_COEFF") == 0) {
        if (!CalcIsCoeff(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "IS_ZERO") == 0) {
        if (!CalcIsZero(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "CLONE") == 0) {
        if (!CalcClone(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "ADD") == 0) {
        if (!CalcAdd(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "MUL") == 0) {
        if (!CalcMul(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "NEG") == 0) {
        if (!CalcNeg(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "SUB") == 0) {
        if (!CalcSub(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "IS_EQ") == 0) {
        if (!CalcIsEq(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "DEG") == 0) {
        if (!CalcDeg(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "PRINT") == 0) {
        if (!CalcPrint(stack))
            ErrorStackUnderflow(lineIndex);
    } else if (strcmp(string, "POP") == 0) {
        if (!CalcPop(stack))
            ErrorStackUnderflow(lineIndex);
    } else {
        // Komenda potencjalnie posiada argument.
        ParseArgumentCommand(string, stack, lineIndex);
    }
}

/**
 * Główna funkcja kalkulatora wielomianów.
 * Zarządza wczytywaniem linii wejściowych oraz
 * parsuje wielomiany i wywołuje odpowiednie komendy.
 * @return kod wyjścia programu
 */
int main() {
    Stack stack = StackCreate(STACK_STARTING_SIZE);
    int lineIndex = 1;

    char *buffer = NULL;
    size_t bufferSize = 0;
    ssize_t getlineSize = safeGetline(&buffer, &bufferSize);

    while (getlineSize != -1) {
        // strlen(buffer) != getlineSize wyklucza linie z '\0' wewnątrz.
        if (strlen(buffer) != (size_t) getlineSize) {
            if (isStringCommand(buffer))
                ErrorWrongCommand(lineIndex);
            else
                ErrorWrongPoly(lineIndex);
        } else if (!isStringEmpty(buffer) && buffer[0] != COMMENT_CHAR) {
            if (isStringCommand(buffer)) {
                ParseCommand(buffer, &stack, lineIndex);
            } else {
                if (!isPolyValid(buffer)) {
                    ErrorWrongPoly(lineIndex);
                } else {
                    errno = 0;
                    char *bufferCopy = buffer;
                    Poly p = ParsePoly(&bufferCopy);

                    if (errno == ERANGE) {
                        PolyDestroy(&p);
                        ErrorWrongPoly(lineIndex);
                    } else {
                        StackPush(&stack, p);
                    }
                }
            }
        }

        // Wczytanie następnej linii.
        lineIndex++;
        getlineSize = safeGetline(&buffer, &bufferSize);
    }

    free(buffer);
    StackDestroy(&stack);

    return 0;
}