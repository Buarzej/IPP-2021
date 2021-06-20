/** @file
  Implementacja stosu wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#include <stdlib.h>
#include "stack.h"

/** Mnożnik aktualnego rozmiaru tablicy stosu przy realokacji. */
#define STACK_REALLOC_MULTIPLIER 2

Stack StackCreate(int size) {
    Stack stack;
    stack.top = -1;
    stack.arraySize = size;
    stack.array = (Poly *) malloc(stack.arraySize * sizeof(Poly));

    // Sprawdzenie poprawności alokacji.
    if (stack.array == NULL) exit(1);
    return stack;
}

void StackDestroy(Stack *stack) {
    for (int i = 0; i <= stack->top; i++)
        PolyDestroy(&stack->array[i]);
    free(stack->array);
}

void StackPush(Stack *stack, Poly p) {
    stack->array[++stack->top] = p;

    if (StackIsFull(stack)) {
        stack->arraySize *= STACK_REALLOC_MULTIPLIER;
        stack->array = (Poly *) realloc(stack->array,
                                        stack->arraySize * sizeof(Poly));
        if (stack->array == NULL)
            exit(1);
    }
}

Poly StackPop(Stack *stack) {
    assert(!StackIsEmpty(stack));
    return stack->array[stack->top--];
}

Poly StackTop(Stack *stack) {
    assert(!StackIsEmpty(stack));
    return stack->array[stack->top];
}

Poly StackSecond(Stack *stack) {
    assert(StackHasNItems(stack, 2));
    return stack->array[stack->top - 1];
}