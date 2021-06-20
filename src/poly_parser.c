/** @file
  Implementacja klasy parsującej wielomiany rzadkie wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "poly.h"
#include "utilities.h"
#include "poly_parser.h"

/** Początkowy rozmiar tablicy jednomianów. */
#define MONOS_STARTING_SIZE 8

/**
 * Konwertuje tekst na wykładnik jednomianu.
 * Wejściowy tekst musi zaczynać się od cyfry będącej pierwszą
 * cyfrą szukanego wykładnika. Funkcja ustawia wskaźnik tekstu
 * wskazywanego przez @p string na pierwszy znak po wykładniku.
 * @param[in,out] string : wskaźnik na tekst do sparsowania
 * @return wykładnik jednomianu
 */
static poly_exp_t ParseExp(char **string) {
    assert(isdigit((int) *string[0]));

    poly_exp_t result;
    char *remaining;
    long tempResult = strtol(*string, &remaining, 10);
    *string = remaining;

    if (tempResult > INT_MAX || tempResult < 0) {
        result = 0;
        errno = ERANGE;
    } else {
        result = (poly_exp_t) tempResult;
    }

    return result;
}

Mono ParseMono(char **string) {
    assert(*string[0] == '(');

    (*string)++; // Pomijamy nawias otwierający jednomian.
    Poly resultPoly = ParsePoly(string);
    (*string)++; // Pomijamy przecinek.
    poly_exp_t resultExp = ParseExp(string);
    (*string)++; // Pomijamy nawias zamykający jednomian.

    // Wykładnik potęgi przy zerowym współczynniku musi być zerowy.
    if (PolyIsZero(&resultPoly))
        resultExp = 0;

    return MonoFromPoly(&resultPoly, resultExp);
}

/**
 * Konwertuje tekst zaczynający się liczbą na wielomian będący
 * współczynnikiem.
 * Funkcja ustawia wskaźnik tekstu wskazywanego przez @p string
 * na pierwszy znak po sparsowanym współczynniku wielomianu.
 * @param[in,out] string : wskaźnik na tekst do sparsowania
 * @return wielomian będący współczynnikiem
 */
static Poly ParsePolyCoeff(char **string) {
    char *remaining;
    poly_coeff_t resultCoeff = strtol(*string, &remaining, 10);
    *string = remaining;
    return PolyFromCoeff(resultCoeff);
}

Poly ParsePoly(char **string) {
    if (isdigit((int) *string[0]) || *string[0] == '-') {
        // Wielomian jest współczynnikiem.
        return ParsePolyCoeff(string);
    } else {
        // Wielomian jest sumą jednomianów.
        Mono *monos = SafeMonoMalloc(MONOS_STARTING_SIZE);
        size_t monosSize = MONOS_STARTING_SIZE, monosCount = 0;
        do {
            if (*string[0] == '+')
                (*string)++;

            Mono currMono = ParseMono(string);
            monos[monosCount++] = currMono;
            if (monosCount == monosSize)
                monos = SafeMonoRealloc(monos, &monosSize);
        } while (*string[0] == '+');

        Poly result = PolyAddMonos(monosCount, monos);
        free(monos);
        return result;
    }
}

bool isPolyValid(const char *string) {
    if (isdigit((int) string[0]) || string[0] == '-') {
        // Sprawdzenie, czy wielomian jest samym minusem.
        if (string[0] == '-' && !isdigit((int) string[1]))
            return false;

        // Wielomian jest współczynnikiem i musi zawierać tylko cyfry.
        for (size_t i = 1; string[i] != '\n'; i++)
            if (!isdigit((int) string[i]))
                return false;
    } else if (string[0] == '(') {
        int bracketCount = 1, commaCount = 0;
        for (size_t i = 1; string[i] != '\n'; i++) {
            if (string[i] == '(') {
                // Nawias otwierający występuje tylko po '+' lub '('.
                if (string[i - 1] != '+' && string[i - 1] != '(')
                    return false;
                bracketCount++;
            } else if (string[i] == '-') {
                // Minus występuje tylko po '('.
                if (string[i - 1] != '(')
                    return false;
            } else if (isdigit((int) string[i])) {
                // Cyfra nie może wystąpić po '+' ani ')'.
                if (string[i - 1] == '+' || string[i - 1] == ')')
                    return false;
            } else if (string[i] == ',') {
                // Przecinek występuje tylko po ')' albo cyfrze.
                if (string[i - 1] != ')' && !isdigit((int) string[i - 1]))
                    return false;
                commaCount++;
            } else if (string[i] == ')') {
                // Nawias zamykający występuje tylko po cyfrze.
                if (!isdigit((int) string[i - 1]))
                    return false;
                bracketCount--;

                // Sprawdzenie prawidłowej liczby przecinków.
                if (--commaCount != 0)
                    return false;
            } else if (string[i] == '+') {
                // Plus występuje tylko między ')' a '('.
                if (string[i - 1] != ')' || string[i + 1] != '(')
                    return false;
            } else {
                // Wielomian zawiera niedozwolony znak.
                return false;
            }

            // Sprawdzenie prawidłowej kolejności nawiasów.
            if (bracketCount < 0)
                return false;
        }

        // Sprawdzenie prawidłowej liczby nawiasów.
        if (bracketCount != 0)
            return false;
    } else {
        // Niepoprawny pierwszy znak wielomianu.
        return false;
    }

    return true;
}
