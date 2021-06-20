/** @file
  Interfejs klasy stosu wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include "poly.h"

/**
 * Struktura przechowująca stos wielomianów.
 * Jeśli `top == -1`, stos jest pusty. Tablica przechowująca elementy
 * stosu jest automatycznie realokowana w razie potrzeby.
 */
typedef struct Stack {
    int top; ///< indeks najwyższego elementu
    int arraySize; ///< rozmiar alokacji tablicy z elementami stosu
    Poly* array; ///< tablica przechowująca elementy stosu
} Stack;

/**
 * Sprawdza, czy stos nie zawiera żadnych elementów.
 * @param[in] stack : stos
 * @return Czy stos jest pusty?
 */
static inline bool StackIsEmpty(Stack *stack) {
    return stack->top == -1;
}

/**
 * Sprawdza, czy liczba elementów stosu osiągnęła
 * limit tablicy i jest wymagana realokacja.
 * @param[in] stack : stos
 * @return Czy stos jest zapełniony?
 */
static inline bool StackIsFull(Stack *stack) {
    return stack->top == stack->arraySize - 1;
}

/**
 * Sprawdza, czy stos zawiera przynajmniej @p n elementów.
 * @param[in] stack : stos
 * @param[in] n : liczba elementów
 * @return Czy stos zawiera co najmniej @p n elementów?
 */
static inline bool StackHasNItems(Stack *stack, int n) {
    return stack->top >= n - 1;
}

/**
 * Tworzy pusty stos o podanym początkowym rozmiarze tablicy.
 * @param[in] size : początkowy rozmiar tablicy
 * @return pusty stos
 */
Stack StackCreate(int size);

/**
 * Usuwa stos i wszystkie jego elementy z pamięci.
 * @param[in] stack : stos
 */
void StackDestroy(Stack *stack);

/**
 * Dodaje wielomian @p p na wierzch stosu.
 * @param[in] stack : stos
 * @param[in] p : wielomian
 */
void StackPush(Stack *stack, Poly p);

/**
 * Zwraca i usuwa wielomian z wierzchu niepustego stosu.
 * @param[in] stack : stos
 * @return usunięty wielomian
 */
Poly StackPop(Stack *stack);

/**
 * Zwraca wielomian z wierzchu niepustego stosu.
 * @param[in] stack : stos
 * @return wielomian z wierzchu stosu
 */
Poly StackTop(Stack *stack);

/**
 * Zwraca wielomian znajdujący się pod wierzchołkiem stosu
 * zawierającego przynajmniej dwa elementy.
 * @param[in] stack : stos
 * @return wielomian pod wierzchołkiem stosu
 */
Poly StackSecond(Stack *stack);

#endif //POLYNOMIALS_STACK_H
