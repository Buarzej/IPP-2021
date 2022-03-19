/** @file
  Interfejs klasy komend kalkulatora wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#ifndef POLYNOMIALS_CALC_COMMANDS_H
#define POLYNOMIALS_CALC_COMMANDS_H

/**
 * Wstawia na wierzch stosu wielomian tożsamościowo równy zeru.
 * @param[in] stack : stos
 */
void CalcZero(Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchu stosu jest
 * współczynnikiem i wypisuje na standardowe wyjście 0 lub 1.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcIsCoeff(const Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchu stosu jest tożsamościowo
 * równy zeru i wypisuje na standardowe wyjście 0 lub 1.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcIsZero(const Stack *stack);

/**
 * Wstawia na stos kopię wielomianu z wierzchu.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcClone(Stack *stack);

/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je
 * i wstawia na wierzch stosu ich sumę.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcAdd(Stack *stack);

/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je
 * i wstawia na wierzch stosu ich iloczyn.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcMul(Stack *stack);

/**
 * Neguje wielomian na wierzchu stosu.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcNeg(Stack *stack);

/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem,
 * usuwa je i wstawia na wierzch stosu otrzymaną różnicę.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcSub(Stack *stack);

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe
 * i wypisuje na standardowe wyjście 0 lub 1.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcIsEq(const Stack *stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu
 * (−1 dla wielomianu tożsamościowo równego zeru).
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcDeg(const Stack *stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną
 * o numerze @p varIdx (−1 dla wielomianu tożsamościowo równego zeru).
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @param[in] varIdx : indeks zmiennej
 * @return Czy operacja się powiodła?
 */
bool CalcDegBy(const Stack *stack, size_t varIdx);

/**
 * Wylicza wartość wielomianu w punkcie @p x, usuwa wielomian
 * z wierzchu stosu i wstawia na stos wynik operacji.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @param[in] x : wartość argumentu @f$x@f$
 * @return Czy operacja się powiodła?
 */
bool CalcAt(Stack *stack, poly_coeff_t x);

/**
 * Zdejmuje z wierzchu stosu wielomian @f$p@f$ oraz wielomiany @f$q_{k - 1},
 * q_{k - 2}, ..., q_0@f$ i umieszcza na stosie wynik operacji złożenia.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @param[in] k : liczba wielomianów do podstawienia
 * @return Czy operacja się powiodła?
 */
bool CalcCompose(Stack *stack, size_t k);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchu stosu.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcPrint(const Stack *stack);

/**
 * Usuwa wielomian z wierzchu stosu.
 * Zwraca `true` lub `false`, w zależności czy operacja się powiodła.
 * @param[in] stack : stos
 * @return Czy operacja się powiodła?
 */
bool CalcPop(Stack *stack);

#endif //POLYNOMIALS_CALC_COMMANDS_H
