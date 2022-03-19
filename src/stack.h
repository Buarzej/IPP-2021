/** @file
  Interfejs klasy stosu wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include <stdlib.h>
#include "poly.h"

/** Mnożnik aktualnego rozmiaru tablicy stosu przy realokacji. */
#define STACK_REALLOC_MULTIPLIER 2

/**
 * Struktura przechowująca stos wielomianów.
 * Tablica przechowująca elementy stosu jest
 * automatycznie realokowana w razie potrzeby.
 */
typedef struct Stack {
    size_t top; ///< liczba elementów wrzuconych na stos
    size_t arraySize; ///< rozmiar alokacji tablicy z elementami stosu
    Poly* array; ///< tablica przechowująca elementy stosu
} Stack;

/**
 * Sprawdza, czy stos nie zawiera żadnych elementów.
 * @param[in] stack : stos
 * @return Czy stos jest pusty?
 */
static inline bool StackIsEmpty(const Stack *stack) {
    return stack->top == 0;
}

/**
 * Sprawdza, czy liczba elementów stosu osiągnęła
 * limit tablicy i jest wymagana realokacja.
 * @param[in] stack : stos
 * @return Czy stos jest zapełniony?
 */
static inline bool StackIsFull(const Stack *stack) {
    return stack->top == stack->arraySize;
}

/**
 * Sprawdza, czy stos zawiera przynajmniej @p n elementów.
 * @param[in] stack : stos
 * @param[in] n : liczba elementów
 * @return Czy stos zawiera co najmniej @p n elementów?
 */
static inline bool StackHasNItems(const Stack *stack, size_t n) {
    return stack->top >= n;
}

/**
 * Tworzy pusty stos o podanym początkowym rozmiarze tablicy.
 * @param[in] size : początkowy rozmiar tablicy
 * @return pusty stos
 */
static inline Stack StackCreate(size_t size) {
    Stack stack;
    stack.top = 0;
    stack.arraySize = size;
    stack.array = (Poly *) malloc(stack.arraySize * sizeof(Poly));

    // Sprawdzenie poprawności alokacji.
    if (stack.array == NULL) exit(1);
    return stack;
}

/**
 * Usuwa stos i wszystkie jego elementy z pamięci.
 * @param[in] stack : stos
 */
static inline void StackDestroy(const Stack *stack) {
    for (size_t i = 0; i < stack->top; i++)
        PolyDestroy(&stack->array[i]);
    free(stack->array);
}

/**
 * Dodaje wielomian @p p na wierzch stosu.
 * @param[in] stack : stos
 * @param[in] p : wielomian
 */
static inline void StackPush(Stack *stack, Poly p) {
    stack->array[stack->top++] = p;

    if (StackIsFull(stack)) {
        stack->arraySize *= STACK_REALLOC_MULTIPLIER;
        stack->array = (Poly *) realloc(stack->array,
                                        stack->arraySize * sizeof(Poly));
        if (stack->array == NULL)
            exit(1);
    }
}

/**
 * Zwraca i usuwa wielomian z wierzchu niepustego stosu.
 * @param[in] stack : stos
 * @return usunięty wielomian
 */
static inline Poly StackPop(Stack *stack) {
    assert(!StackIsEmpty(stack));
    return stack->array[--stack->top];
}

/**
 * Zwraca wielomian z wierzchu niepustego stosu.
 * @param[in] stack : stos
 * @return wielomian z wierzchu stosu
 */
static inline Poly StackTop(const Stack *stack) {
    assert(!StackIsEmpty(stack));
    return stack->array[stack->top - 1];
}

/**
 * Zwraca wielomian znajdujący się pod wierzchołkiem stosu
 * zawierającego przynajmniej dwa elementy.
 * @param[in] stack : stos
 * @return wielomian pod wierzchołkiem stosu
 */
static inline Poly StackSecond(const Stack *stack) {
    assert(StackHasNItems(stack, 2));
    return stack->array[stack->top - 2];
}

#endif //POLYNOMIALS_STACK_H
