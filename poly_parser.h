/** @file
  Interfejs klasy parsującej wielomiany rzadkie wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#ifndef POLYNOMIALS_POLY_PARSER_H
#define POLYNOMIALS_POLY_PARSER_H

#include <stdbool.h>
#include "poly.h"

/**
 * Konwertuje tekst rozpoczynający się od fragmentu postaci (coeff,exp)
 * na jednomian, gdzie coeff to jego współczynnik, a exp - wykładnik.
 * Funkcja ustawia wskaźnik tekstu wskazywanego przez @p string
 * na pierwszy znak po nawiasie zamykającym jednomian.
 * @param[in,out] string : wskaźnik na tekst do sparsowania
 * @return jednomian
 */
Mono ParseMono(char **string);

/**
 * Konwertuje linię tekstu będącą wielomianem na wielomian.
 * Funkcja ustawia wskaźnik tekstu wskazywanego przez @p string
 * na pierwszy znak po sparsowanym wielomianie.
 * @param[in,out] string : wskaźnik na tekst do sparsowania
 * @return wielomian
 */
Poly ParsePoly(char **string);

/**
 * Sprawdza, czy linia tekstu reprezentuje prawidłowy wielomian.
 * @param[in] string : tekst do sprawdzenia
 * @return Czy linia tekstu przedstawia wielomian?
 */
bool isPolyValid(const char *string);

#endif //POLYNOMIALS_POLY_PARSER_H
