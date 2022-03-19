/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych

  Założenia:
  - wielomiany są posortowane względem współczynników potęg
  - współczynnik przy niezerowej potędze jest niezerowy
  - wielomiany zawierają tablicę jednomianów o parami różnych wykładnikach
  - kiedy możemy, wielomiany zamieniamy na typ coeff

  @author Błażej Wilkoławski
  @date 2021
*/

#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "poly.h"

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++)
            MonoDestroy(&p->arr[i]);
        free(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    Mono *resultMonos = SafeMonoMalloc(p->size);

    for (size_t i = 0; i < p->size; i++)
        resultMonos[i] = MonoClone(&p->arr[i]);

    return (Poly) {.size = p->size, .arr = resultMonos};
}

/**
 * Konwertuje wielomian będący współczynnikiem @f$c@f$
 * do wielomianu postaci @f$c \cdot x_0^0@f$.
 * @param[in] p : wielomian typu coeff
 * @return @f$p \cdot x_0^0@f$
 */
static Poly ConvertCoeff(const Poly *p) {
    assert(PolyIsCoeff(p));

    Mono *arr = SafeMonoMalloc(1);
    arr[0] = MonoFromPoly(p, 0);
    return (Poly) {.size = 1, .arr = arr};
}

/**
 * Konwertuje wielomian do wielomianu będącego współczynnikiem, o ile jest
 * to możliwe. W przeciwnym wypadku zwraca niezmieniony wejściowy wielomian.
 * @param[in] p : wielomian
 * @return przekonwertowany lub niezmieniony wielomian
 */
static Poly ConvertToCoeff(Poly *p) {
    if (!PolyIsCoeff(p) && p->size == 1 && p->arr[0].exp == 0 &&
        PolyIsCoeff(&p->arr[0].p)) {
        Poly result = PolyFromCoeff(p->arr[0].p.coeff);
        PolyDestroy(p);

        return result;
    }
    return *p;
}

/**
 * Dodaje dwa wielomiany niebędące współczynnikami.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
static Poly PolyAddNotCoeffs(const Poly *p, const Poly *q) {
    Mono *resultMonos = SafeMonoMalloc(p->size + q->size);
    size_t pIndex = 0, qIndex = 0, resultSize = 0;

    // Przechodzimy po kolejnych jednomianach wielomianów p i q,
    // sumując odpowiednie współczynniki.
    while (pIndex != p->size || qIndex != q->size) {
        if (pIndex == p->size) {
            resultMonos[resultSize++] = MonoClone(&q->arr[qIndex]);
            qIndex++;
        } else if (qIndex == q->size) {
            resultMonos[resultSize++] = MonoClone(&p->arr[pIndex]);
            pIndex++;
        } else {
            Mono pMono = p->arr[pIndex];
            Mono qMono = q->arr[qIndex];

            if (pMono.exp < qMono.exp) {
                resultMonos[resultSize++] = MonoClone(&pMono);
                pIndex++;
            } else if (pMono.exp > qMono.exp) {
                resultMonos[resultSize++] = MonoClone(&qMono);
                qIndex++;
            } else {
                Poly newPoly = PolyAdd(&pMono.p, &qMono.p);
                pIndex++;
                qIndex++;

                // Gdy współczynniki się wyzerowały, nie zapisujemy jednomianu.
                if (!PolyIsZero(&newPoly))
                    resultMonos[resultSize++] = MonoFromPoly(&newPoly,
                                                             pMono.exp);
            }
        }
    }

    // Jeśli wszystkie jednomiany się skróciły, zwracamy wielomian zerowy.
    if (resultSize == 0) {
        free(resultMonos);
        return PolyZero();
    }

    Poly result = {.size = resultSize, .arr = resultMonos};

    // Przed zwróceniem konwertujemy wynik na typ coeff, o ile to możliwe.
    return ConvertToCoeff(&result);
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) || PolyIsZero(q))
        return PolyIsZero(p) ? PolyClone(q) : PolyClone(p);

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->coeff + q->coeff);

    if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        // Pomocniczo przemnażamy wielomian będący współczynnikiem przez x^0.
        Poly converted = ConvertCoeff(p);
        Poly result = PolyAddNotCoeffs(&converted, q);
        PolyDestroy(&converted);

        return result;
    }

    if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        // Pomocniczo przemnażamy wielomian będący współczynnikiem przez x^0.
        Poly converted = ConvertCoeff(q);
        Poly result = PolyAddNotCoeffs(p, &converted);
        PolyDestroy(&converted);

        return result;
    }

    return PolyAddNotCoeffs(p, q);
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (count == 0) {
        if (monos != NULL)
            free(monos);
        return PolyZero();
    }

    // Sortujemy tablicę względem wykładników jednomianów.
    qsort(monos, count, sizeof(Mono), compareMonos);

    Mono *resultMonos = SafeMonoMalloc(count);
    size_t index = 0;
    resultMonos[index] = monos[0];

    // Przechodzimy po posortowanej tablicy, sumując współczynniki
    // przy jednomianach o tym samym wykładniku.
    for (size_t i = 1; i < count; i++) {
        if (resultMonos[index].exp == monos[i].exp) {
            // Brak zmiany wykładnika, sumujemy współczynniki.
            Poly currPoly = resultMonos[index].p;
            resultMonos[index].p = PolyAdd(&currPoly, &monos[i].p);
            MonoDestroy(&monos[i]);
            PolyDestroy(&currPoly);
        } else {
            // Przy zmianie wykładnika zapisujemy zsumowane współczynniki
            // do tablicy wynikowej, przy czym nadpisujemy jednomiany zerowe.
            if (!PolyIsZero(&resultMonos[index].p))
                index++;
            resultMonos[index] = monos[i];
        }
    }

    // Oddzielnie rozpatrujemy ostatni wykładnik z posortowanej tablicy.
    if (!PolyIsZero(&resultMonos[index].p))
        index++;

    free(monos);
    size_t resultSize = index;

    // Jeśli wszystkie jednomiany się skróciły, zwracamy wielomian zerowy.
    if (resultSize == 0) {
        free(resultMonos);
        return PolyZero();
    }

    Poly result = {.size = resultSize, .arr = resultMonos};

    // Przed zwróceniem konwertujemy wynik na typ coeff, o ile to możliwe.
    return ConvertToCoeff(&result);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL)
        return PolyZero();

    // Robimy płytką kopię tablicy monos[] w celu jej posortowania.
    Mono *monosCopy = SafeMonoMalloc(count);
    size_t newCount = 0;
    for (size_t i = 0; i < count; i++)
        // Pomijamy jednomiany tożsamościowo równe zero.
        if (!PolyIsZero(&monos[i].p))
            monosCopy[newCount++] = monos[i];

    return PolyOwnMonos(newCount, monosCopy);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL)
        return PolyZero();

    // Robimy głęboką kopię tablicy monos[] w celu jej posortowania.
    Mono *monosCopy = SafeMonoMalloc(count);
    size_t newCount = 0;
    for (size_t i = 0; i < count; i++)
        // Pomijamy jednomiany tożsamościowo równe zero.
        if (!PolyIsZero(&monos[i].p))
            monosCopy[newCount++] = MonoClone(&monos[i]);

    return PolyOwnMonos(newCount, monosCopy);
}

Poly PolyMulByCoeff(const Poly *p, poly_coeff_t coeff) {
    if (coeff == 0)
        return PolyZero();

    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff * coeff);

    Mono *resultMonos = SafeMonoMalloc(p->size);

    size_t resultSize = 0;
    for (size_t i = 0; i < p->size; i++) {
        Mono newMono = {.exp = p->arr[i].exp, .p = PolyMulByCoeff(&p->arr[i].p,
                                                                  coeff)};
        if (!PolyIsZero(&newMono.p))
            resultMonos[resultSize++] = newMono;
    }

    // Sprawdzenie, czy nie doszło do przekroczenia zakresu zmiennej.
    if (resultSize == 0) {
        free(resultMonos);
        return PolyZero();
    }

    return (Poly) {.arr = resultMonos, .size = resultSize};
}

/**
 * Mnoży dwa wielomiany niebędące współczynnikami.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p \cdot q@f$
 */
static Poly PolyMulNotCoeffs(const Poly *p, const Poly *q) {
    // Warunek optymalizujący liczbę alokacji.
    if (p->size > q->size)
        return PolyMulNotCoeffs(q, p);

    Poly result = PolyZero();

    // Przemnażamy kolejne jednomiany wielomianu p przez
    // jednomiany wielomianu q i sumujemy tak powstałe wyniki.
    for (size_t i = 0; i < p->size; i++) {
        Mono *iterationMonos = SafeMonoMalloc(q->size);
        Mono pMono = p->arr[i];

        for (size_t j = 0; j < q->size; j++) {
            Mono qMono = q->arr[j];
            Poly newPoly = PolyMul(&pMono.p, &qMono.p);
            poly_exp_t newExp = MonoGetExp(&pMono) + MonoGetExp(&qMono);

            // Dodatkowy warunek PolyIsZero() sprawdza, czy przy mnożeniu
            // współczynników nie doszło do przekroczenia zakresu zmiennej.
            iterationMonos[j] = PolyIsZero(&newPoly)
                                ? MonoFromPoly(&newPoly, 0)
                                : MonoFromPoly(&newPoly, newExp);
        }

        // Przy każdej iteracji sumujemy wielomian powstały po przemnożeniu
        // jednego jednomianu z p przez wszystkie jednomiany z q.
        Poly temp = PolyOwnMonos(q->size, iterationMonos);
        Poly oldResult = result;
        result = PolyAdd(&oldResult, &temp);

        PolyDestroy(&oldResult);
        PolyDestroy(&temp);
    }

    // Przed zwróceniem konwertujemy wynik na typ coeff, o ile to możliwe.
    return ConvertToCoeff(&result);
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->coeff * q->coeff);

    if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return PolyMulByCoeff(q, p->coeff);
    }

    if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyMulByCoeff(p, q->coeff);
    }

    return PolyMulNotCoeffs(p, q);
}

Poly PolyPow(const Poly *p, poly_exp_t n) {
    if (n == 0)
        return PolyFromCoeff(1);
    else if (n == 1)
        return PolyClone(p);

    if (PolyIsZero(p))
        return PolyZero();

    if (PolyIsCoeff(p))
        return PolyFromCoeff(fastPow(p->coeff, n));

    Poly multiplier = PolyClone(p);
    Poly result = PolyFromCoeff(1);

    // Algorytm szybkiego potęgowania.
    while (n > 0) {
        if (n % 2 != 0) {
            // Przemnożenie wyniku przez p.
            Poly oldResult = result;
            result = PolyMul(&oldResult, &multiplier);
            PolyDestroy(&oldResult);
        }

        // Podniesienie mnożnika do kwadratu.
        Poly oldMultiplier = multiplier;
        multiplier = PolyMul(&oldMultiplier, &oldMultiplier);
        PolyDestroy(&oldMultiplier);

        n /= 2;
    }

    PolyDestroy(&multiplier);
    return result;
}

/**
 * Pomocnicza funkcja wykonująca operację składania wielomianów
 * zawierająca dodatkową informację o indeksie aktualnej zmiennej.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] k : liczba wielomianów do podstawienia
 * @param[in] q : lista wielomianów do podstawienia
 * @param[in] varIdx : indeks aktualnie rozpatrywanej zmiennej
 * @return
 */
static Poly PolyComposeHelper(const Poly *p, size_t k, const Poly q[], size_t varIdx) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    Poly result = PolyZero();

    for (size_t i = 0; i < p->size; i++) {
        Poly newPoly;

        // Podstawienie odpowiedniego wielomianu za aktualną zmienną.
        if (k > varIdx)
            newPoly = PolyPow(&q[varIdx], MonoGetExp(&p->arr[i]));
        else if (MonoGetExp(&p->arr[i]) == 0)
            newPoly = PolyFromCoeff(1);
        else
            newPoly = PolyZero();

        // Podstawienie pozostałych zmiennych aktualnego jednomianu.
        Poly innerPoly = PolyComposeHelper(&p->arr[i].p, k, q, varIdx + 1);

        Poly oldNewPoly = newPoly;
        newPoly = PolyMul(&oldNewPoly, &innerPoly);

        Poly oldResult = result;
        result = PolyAdd(&oldResult, &newPoly);

        PolyDestroy(&newPoly);
        PolyDestroy(&innerPoly);
        PolyDestroy(&oldNewPoly);
        PolyDestroy(&oldResult);
    }

    return result;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    return PolyComposeHelper(p, k, q, 0);
}

Poly PolyNeg(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(-1 * p->coeff);

    Mono *resultMonos = SafeMonoMalloc(p->size);

    for (size_t i = 0; i < p->size; i++) {
        Mono newMono = {.exp = p->arr[i].exp, .p = PolyNeg(&p->arr[i].p)};
        resultMonos[i] = newMono;
    }

    return (Poly) {.size = p->size, .arr = resultMonos};
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly negated = PolyNeg(q);
    Poly result = PolyAdd(p, &negated);
    PolyDestroy(&negated);

    return result;
}

poly_exp_t PolyDegBy(const Poly *p, size_t varIdx) {
    if (PolyIsZero(p)) return -1;
    if (PolyIsCoeff(p)) return 0;

    poly_exp_t deg = 0;
    for (size_t i = 0; i < p->size; i++) {
        if (varIdx == 0) {
            // Jesteśmy na odpowiednim indeksie zmiennej, obliczamy stopień.
            poly_exp_t currExp = MonoGetExp(&p->arr[i]);
            if (currExp > deg)
                deg = currExp;
        } else {
            // Rekurencja zagłębia się do kolejnego indeksu zmiennej.
            poly_exp_t innerDeg = PolyDegBy(&p->arr[i].p, varIdx - 1);
            if (innerDeg > deg)
                deg = innerDeg;
        }
    }

    return deg;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) return -1;
    if (PolyIsCoeff(p)) return 0;

    poly_exp_t deg = 0;
    for (size_t i = 0; i < p->size; i++) {
        // Sumujemy aktualny i wewnętrzny stopień jednomianu.
        poly_exp_t monoDeg = MonoGetExp(&p->arr[i]) + PolyDeg(&p->arr[i].p);
        if (monoDeg > deg)
            deg = monoDeg;
    }

    return deg;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return p->coeff == q->coeff;

    if (PolyIsCoeff(p) != PolyIsCoeff(q))
        return false;

    if (p->size != q->size)
        return false;

    // Sprawdzamy równość wszystkich jednomianów.
    for (size_t i = 0; i < p->size; i++) {
        if (p->arr[i].exp != q->arr[i].exp ||
            !PolyIsEq(&p->arr[i].p, &q->arr[i].p))
            return false;
    }

    return true;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    // Przechodzimy po wszystkich jednomianach, podstawiając odpowiednie x_0
    // i sumując tak powstałe wyrazy.
    Poly result = PolyZero();
    for (size_t i = 0; i < p->size; i++) {
        Mono currMono = p->arr[i];
        Poly temp = PolyMulByCoeff(&currMono.p,
                                   fastPow(x, MonoGetExp(&currMono)));

        Poly oldResult = result;
        result = PolyAdd(&oldResult, &temp);

        PolyDestroy(&oldResult);
        PolyDestroy(&temp);
    }

    return result;
}

void PolyPrint(const Poly *p) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    } else {
        printf("(");
        for (size_t i = 0; i < p->size; i++) {
            PolyPrint(&p->arr[i].p);
            printf(",%d", p->arr[i].exp);

            if (i + 1 != p->size)
                printf(")+(");
        }
        printf(")");
    }
}