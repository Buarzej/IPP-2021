/** @file
  Interfejs pomocniczej klasy używanej w klasie
  kalkulatora wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#ifndef POLYNOMIALS_UTILITIES_H
#define POLYNOMIALS_UTILITIES_H

#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "poly.h"

/** Mnożnik aktualnego rozmiaru tablicy jednomianów przy realokacji. */
#define MONO_REALLOC_MULTIPLIER 2

/**
 * Sprawdza, czy linia tekstu jest pusta.
 * @param[in] string : tekst do sprawdzenia
 * @return Czy linia tekstu jest pusta?
 */
static inline bool isStringEmpty(const char *string) {
    return string[0] == '\n';
}

/**
 * Sprawdza, czy linia tekstu reprezentuje komendę kalkulatora wielomianów.
 * @param[in] string : tekst do sprawdzenia
 * @return Czy linia tekstu przedstawia komendę kalkulatora?
 */
static inline bool isStringCommand(const char *string) {
    return isalpha((int) string[0]);
}

/**
 * Funkcja wykonująca szybkie potęgowanie.
 * @param[in] x : podstawa @f$x@f$
 * @param[in] n : wykładnik @f$n@f$
 * @return @f$x^n@f$
 */
static inline poly_coeff_t fastPow(poly_coeff_t x, poly_exp_t n) {
    poly_coeff_t result = 1;
    while (n > 0) {
        if (n % 2 != 0)
            result *= x;
        x *= x;
        n /= 2;
    }
    return result;
}

/**
 * Komparator jednomianów porównujący ich wykładniki,
 * zgodny z interfejsem komparatorów używanych w qsort.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return Czy @f$p@f$ ma większy wykładnik niż @f$q@f$?
 */
static inline int compareMonos(const void *p, const void *q) {
    Mono pMono = *(Mono *) p;
    Mono qMono = *(Mono *) q;

    if (pMono.exp > qMono.exp) return 1;
    else if (pMono.exp == qMono.exp) return 0;
    else return -1;
}

/**
 * Bezpieczna alokacja pamięci tablicy jednomianów.
 * @param[in] size : rozmiar tablicy do zaalokowania
 * @return zaalokowana tablica
 */
static inline Mono *SafeMonoMalloc(size_t size) {
    Mono *allocated = malloc(size * sizeof(Mono));
    if (allocated == NULL) exit(1);
    return allocated;
}

/**
 * Bezpieczna realokacja pamięci tablicy jednomianów.
 * @param[in] toRealloc : tablica do realokacji
 * @param[in,out] currSize : wskaźnik na aktualny rozmiar tablicy
 * @return zrealokowana tablica
 */
static inline Mono *SafeMonoRealloc(Mono *toRealloc, size_t *currSize) {
    *currSize *= MONO_REALLOC_MULTIPLIER;
    Mono *reallocated = realloc(toRealloc, *currSize * sizeof(Mono));
    if (reallocated == NULL) exit(1);
    return reallocated;
}

#endif //POLYNOMIALS_UTILITIES_H