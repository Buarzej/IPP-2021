/** @file
  Implementacja komend kalkulatora wielomianów rzadkich wielu zmiennych

  @author Błażej Wilkoławski
  @date 2021
*/

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "utilities.h"
#include "calc_commands.h"

void CalcZero(Stack *stack) {
    StackPush(stack, PolyZero());
}

bool CalcIsCoeff(const Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    int result = PolyIsCoeff(&p) ? 1 : 0;
    printf("%d\n", result);
    return true;
}

bool CalcIsZero(const Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    int result = PolyIsZero(&p) ? 1 : 0;
    printf("%d\n", result);
    return true;
}

bool CalcClone(Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    StackPush(stack, PolyClone(&p));
    return true;
}

bool CalcAdd(Stack *stack) {
    if (!StackHasNItems(stack, 2))
        return false;

    Poly p = StackPop(stack);
    Poly q = StackPop(stack);
    Poly result = PolyAdd(&p, &q);
    StackPush(stack, result);

    PolyDestroy(&p);
    PolyDestroy(&q);
    return true;
}

bool CalcMul(Stack *stack) {
    if (!StackHasNItems(stack, 2))
        return false;

    Poly p = StackPop(stack);
    Poly q = StackPop(stack);
    Poly result = PolyMul(&p, &q);
    StackPush(stack, result);

    PolyDestroy(&p);
    PolyDestroy(&q);
    return true;
}

bool CalcNeg(Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackPop(stack);
    StackPush(stack, PolyNeg(&p));

    PolyDestroy(&p);
    return true;
}

bool CalcSub(Stack *stack) {
    if (!StackHasNItems(stack, 2))
        return false;

    Poly p = StackPop(stack);
    Poly q = StackPop(stack);
    Poly result = PolySub(&p, &q);
    StackPush(stack, result);

    PolyDestroy(&p);
    PolyDestroy(&q);
    return true;
}

bool CalcIsEq(const Stack *stack) {
    if (!StackHasNItems(stack, 2))
        return false;

    Poly p = StackTop(stack);
    Poly q = StackSecond(stack);
    printf("%d\n", PolyIsEq(&p, &q));
    return true;
}

bool CalcDeg(const Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    printf("%d\n", PolyDeg(&p));
    return true;
}

bool CalcDegBy(const Stack *stack, size_t varIdx) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    printf("%d\n", PolyDegBy(&p, varIdx));
    return true;
}

bool CalcAt(Stack *stack, poly_coeff_t x) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackPop(stack);
    StackPush(stack, PolyAt(&p, x));

    PolyDestroy(&p);
    return true;
}

bool CalcCompose(Stack *stack, size_t k) {
    if (StackIsEmpty(stack))
        return false;
    Poly p = StackPop(stack);

    if (!StackHasNItems(stack, k)) {
        StackPush(stack, p);
        return false;
    }

    Poly *q = SafePolyMalloc(k);
    for (size_t i = 1; i <= k; i++)
        q[k - i] = StackPop(stack);

    StackPush(stack, PolyCompose(&p, k, q));

    PolyDestroy(&p);
    for (size_t i = 0; i < k; i++)
        PolyDestroy(&q[i]);
    free(q);

    return true;
}

bool CalcPrint(const Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackTop(stack);
    PolyPrint(&p);
    printf("\n");
    return true;
}

bool CalcPop(Stack *stack) {
    if (StackIsEmpty(stack))
        return false;

    Poly p = StackPop(stack);
    PolyDestroy(&p);
    return true;
}

