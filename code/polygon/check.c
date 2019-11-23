#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "boolean.h"
#include "numeric.h"
#include "polygon.h"

void print_array (const real_t* P, size_t n)
{
    size_t i;

    i = 0U;

    if (!P)
        return;

    for (i = 0U; i < n; ++i)
    {
        if (rabs(P[i]) == lambda)
            continue;

        printf("%lu: %f\n", i, P[i]);
    }
}

real_t* read_polygon (const char* filename, size_t* n)
{
    FILE* in;

    real_t* P;

    size_t i;

    in = NULL;

    P = NULL;

    i = 0U;

    if (!(filename && n))
        return NULL;

    *n = 0U;

    in = fopen(filename, "rt");

    if (!in)
        return NULL;

    fscanf(in, " %lu", n);

    P = *n ? malloc(2U * *n * sizeof *P) : NULL;

    if (!P)
        return NULL;

    for (i = 0U; i / 2U < *n; ++i)
        fscanf(in, " %lf", P + i);

    fclose(in);
    in = NULL;

    return P;
}

int main (int argc, char** argv)
{
    size_t n;
    real_t* P;

    n = 0U;
    P = NULL;

    srand((unsigned int)time(NULL));

    P = read_polygon("poly.txt", &n);

    print_array(P, 2U * n);
    printf("\n");

    simplify_polygon(n, P);

    print_array(P, 2U * n);
    printf("\n");

    printf("%d\n", check_polygon(n, P));
    printf("\n");

    printf("%f\n", diameter(n, P, false));

    memset(P, 0, 2U * n * sizeof *P);

    free(P);
    P = NULL;

    return EXIT_SUCCESS;
}
