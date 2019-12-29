/**
 * Program for generating and displaying polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./generate m n
 * where "./generate" is the command of the compiled program, m is the
 * decimal representation of the number of polygons (>= 1) and n is the decimal
 * representation of the number of vertices (>= 3).
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Include standard library headers. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Include package headers. */
#include "include/array.h"
#include "include/boolean.h"
#include "include/numeric.h"
#include "include/polygon.h"
#include "include/playground.h"

/* Define constants for maximal numbers of iterations. */
#define IN_ITER_MAX     1024U
#define OUT_ITER_MAX    1024U
#define ATTEMPT_MAX     1024U

/* Define functions. */

void initialise_generators ()
{
    saved_nn_integer(3U);
    saved_number(1.0 / 12.0);
    saved_comb_function(rmin);
    saved_len_generator(constant_length);
}

void print_formatted (const char* outname, size_t n, const real_t* P, size_t N)
{
    static const size_t prec = 4U;

    FILE* out;

    size_t i;
    size_t j;
    size_t k;

    out = (FILE*)(NULL);

    i = 0U;
    j = 0U;
    k = 0U;

    do
    {
        if (!(outname && n && P && N))
            break;

        out = fopen(outname, "wt");

        if (!out)
            break;

        for (i = 0U; i < N; ++i)
        {
            j = i << 1U;
            fprintf(out, "%.*f", (int)prec, *(P + j * n));
            for (k = 1U; (k >> 1U) < n; ++k)
                fprintf(out, "\t%.*f", (int)prec, *(P + j * n + k));
            fprintf(out, "\n");
        }

        fclose(out);
    }
    while (false);
}

/**
 * The main program.
 *
 * @param argc
 *     Number of arguments given via the command line.  Should be 3.
 *
 * @param argv
 *     Arguments given via the command line.  Should be an array of length 3
 *     such that the first element is the name (command) of the program, the
 *     second element is the decimal representation of the number of polygons
 *     and the third element is th decimal representation of the number of
 *     vertices.
 *
 * @return
 *     Value 0 if the given arguments were correct and no memory allocation
 *     failed; otherwise a value other than 0.
 *
 */
int main (int argc, char** argv)
{
    /* DECLARATION OF VARIABLES */

    /* Number of polygons. */
    size_t N;

    /* Expected number of vertices and true number of vertices. */
    size_t n;
    size_t n_true;

    /* Array of vertices. */
    real_t* P;

    /* Arrays of differences in coordinates. */
    real_t* dx;
    real_t* dy;

    /* Arrays of the edges' lengths and the outer angles. */
    real_t* l;
    real_t* phi;

    /* Value returned by the displaying application. */
    int return_value;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Number of polygons. */
    N = 0U;

    /* Expected number of vertices and true number of vertices. */
    n = 0U;
    n_true = 0U;

    /* Array of vertices. */
    P = (real_t*)(NULL);

    /* Arrays of differences in coordinates. */
    dx = (real_t*)(NULL);
    dy = (real_t*)(NULL);

    /* Arrays of the edges' lengths and the outer angles. */
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Value returned by the displaying application. */
    return_value = 0;

    /* Iteration index. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* Initialise the generating functions. */
    initialise_generators();

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the number of command line arguments is not 2, print the error message
     * and exit with a non-zero value. */
    if (!(argc == 3))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "Exactly two additional command line argument (number of polygons"
            "and number of vertices) must be given.\n"
        );

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the pointer to command line arguments is a null-pointer, print the
     * error message and exit with a non-zero value. */
    if (!argv)
    {
        /* Print the error message. */
        fprintf(stderr, "Command line arguments' array is a null pointer.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the pointers any of the first and the second command line arguments is
     * a null-pointer, print the error message and exit with a non-zero
     * value. */
    if (!(*argv && *(argv + 1U) && *(argv + 2U)))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "At least one of the command line arguments is a null-pointer.\n"
        );

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the number of polygons from the second command line argument. */
    N = (size_t)atoi(*(argv + 1U));

    /* Read the number of vertices from the third command line argument. */
    n = (size_t)atoi(*(argv + 2U));

    /* If the number of polygons is 0, print the error message and exit with a
     * non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, "Number of polygons must be at least 1.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the number of vertices is strictly less than 3, print the error
     * message and exit with a non-zero value. */
    if (n < 3U)
    {
        /* Print the error message. */
        fprintf(stderr, "Number of vertices must be at least 3.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Allocate the memory for the polygons. */
    P = (real_t*)malloc(((N * n) << 1U) * sizeof *P);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!P)
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the array `P` to zeros. */
    memset(P, 0, ((N * n) << 1U) * sizeof *P);

    /* Allocate memory for the arrays of the differences in coordinates, edges'
     * lengths and the outer angles. */
    dx = (real_t*)malloc(N * n * sizeof *dx);
    dy = (real_t*)malloc(N * n * sizeof *dy);
    l = (real_t*)malloc(N * n * sizeof *l);
    phi = (real_t*)malloc(N * n * sizeof *phi);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!(dx && dy && l && phi))
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Deallocate the memory allocated for the arrays of the differences in
         * coordinates, edges' lengths and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(dy);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the arrays of the differences in coordinates, edges' lengths
     * and the outer angles. */
    memset(dx, 0, N * n * sizeof *dx);
    memset(dy, 0, N * n * sizeof *dy);
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *phi);

    /* Save values and pointers to arrays for generating new values. */
    saved_nn_integer(n);
    /*saved_number(1.0 / (real_t)n);*/
    saved_polygon(P);
    saved_lengths(l);
    saved_angles(phi);

    /* Attempt to generate an `n`-gon ATTEMPT_MAX times at the most. */
    for (i = 0U; i < (ATTEMPT_MAX); ++i)
    {
        /* Set the value of number `n_true` to the value of `n`. */
        n_true = n;

        /* Generate an array of `n` points using the `smart_random_polygon`
         * function. */
        smart_random_polygon(
            n,
            P,
            regular_coordinate,
            OUT_ITER_MAX,
            IN_ITER_MAX
        );

        /* Reorder the vertices by flipping the array from index 2 to the
         * end. */
        flip(P + 2U, n - 1U, (sizeof *P) << 1U);

        /* Simplify and check the array for the polygon.  If the array
         * represents a true `n`-gon, break the `for`-loop. */
        if (simplify_check_polygon(&n_true, P))
            if (n_true == n)
                break;

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N * n) << 1U) * sizeof *P);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the original polygon has not been successfully generated, inform the
     * user and exit with a zero-value. */
    if (i >= (ATTEMPT_MAX))
    {
        /* Inform the user. */
        printf("No true %lu-gon found.\n", n);

        /* Clear the memory in the arrays of the differences in coordinates,
         * edges' lengths and the outer angles. */
        memset(dx, 0, N * n * sizeof *dx);
        memset(dy, 0, N * n * sizeof *dy);
        memset(l, 0, N * n * sizeof *l);
        memset(phi, 0, N * n * sizeof *phi);

        /* Deallocate the memory allocated for the arrays of the differences in
         * coordinates, edges' lengths and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(dy);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N * n) << 1U) * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a zero value. */
        exit(EXIT_SUCCESS);
    }

    /* Normalise the original polygon. */
    normalise_polygon(n, P);

    /* Compute the edges' lengths and the outer angles of the original
     * polygon. */
    describe_polygon(n, P, dx, dy, l, phi);

    /* Generate perturbated polygons. */
    for (j = 1U; j < N; ++j)
    {
        /* Attempt to generate an `n`-gon ATTEMPT_MAX times at the most by
         * perturbating the first polygon. */
        for (i = 0U; i < (ATTEMPT_MAX); ++i)
        {
            /* Set the value of number `n_true` to the value of `n`. */
            n_true = n;

            /* Generate an array of `n` points using the `random_polygon`
             * function. */
            random_polygon(n, P + ((j * n) << 1U), perturbate_coordinate);

            /* Reorder the vertices by flipping the array from index 2 to the
             * end. */
            flip(P + ((j * n) << 1U) + 2U, n - 1U, (sizeof *P) << 1U);

            /* Simplify and check the array for the polygon.  If the array
             * represents a true `n`-gon, break the `for`-loop. */
            if (simplify_check_polygon(&n_true, P + ((j * n) << 1U)))
                if (n_true == n)
                    break;

            /* Clear the memory in the array of points. */
            memset(P + ((j * n) << 1U), 0, (n << 1U) * sizeof *P);
        }

        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);

        /* If the perturbated polygon has not been successfully generated,
         * inform the user and exit with a zero-value. */
        if (i >= (ATTEMPT_MAX))
        {
            /* Inform the user. */
            printf("Generation of the perturbated polygon (%lu) failed.\n", j);

            /* Clear the memory in the arrays of the differences in coordinates,
             * edges' lengths and the outer angles. */
            memset(dx, 0, N * n * sizeof *dx);
            memset(dy, 0, N * n * sizeof *dy);
            memset(l, 0, N * n * sizeof *l);
            memset(phi, 0, N * n * sizeof *phi);

            /* Deallocate the memory allocated for the arrays of the differences in
             * coordinates, edges' lengths and the outer angles. */
            free(dx);
            dx = (real_t*)(NULL);
            free(dy);
            dy = (real_t*)(NULL);
            free(l);
            l = (real_t*)(NULL);
            free(phi);
            phi = (real_t*)(NULL);

            /* Clear the memory in the array of points. */
            memset(P, 0, ((N * n) << 1U) * sizeof *P);

            /* Deallocate the memory allocated for the array of points. */
            free(P);
            P = (real_t*)(NULL);

            /* Exit with a zero value. */
            exit(EXIT_SUCCESS);
        }

        /* Normalise the perturbated polygon. */
        normalise_polygon(n, P + ((j * n) << 1U));

        /* Compute the edges' lengths and the outer angles of the original
         * polygon. */
        describe_polygon(
            n,
            P + ((j * n) << 1U),
            dx + j * n,
            dy + j * n,
            l + j * n,
            phi + j * n
        );
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Display the polygons and print the returned value. */
    return_value = display(n, P, N);
    printf("Returned: %d (0x%03X)\n", return_value, return_value);

    print_formatted("test.tsv", n, P, N);

    /* Clear the memory in the arrays of the differences in coordinates,
     * edges' lengths and the outer angles. */
    memset(dx, 0, N * n * sizeof *dx);
    memset(dy, 0, N * n * sizeof *dy);
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *phi);

    /* Deallocate the memory allocated for the arrays of the differences in
     * coordinates, edges' lengths and the outer angles. */
    free(dx);
    dx = (real_t*)(NULL);
    free(dy);
    dy = (real_t*)(NULL);
    free(l);
    l = (real_t*)(NULL);
    free(phi);
    phi = (real_t*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, ((N * n) << 1U) * sizeof *P);

    /* Deallocate the memory allocated for the array of points. */
    free(P);
    P = (real_t*)(NULL);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Return 0 after successfully running the program. */
    return EXIT_SUCCESS;
}
