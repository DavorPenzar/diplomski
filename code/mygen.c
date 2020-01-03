/**
 * Program for generating polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Compile with mathematical constants from "math.h". */
#define _USE_MATH_DEFINES   1

/* Use the DGESVD driver in the `svd_polygon` function from "polygon.h"
 * header. */
#define _USE_SVD_DRIVER 1

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

/* Define a constant for the number of angles of the polygon. */
#define N_ANGLES 16U

/* Define constants for maximal numbers of iterations. */
#define IN_ITER_MAX     1024U
#define OUT_ITER_MAX    1024U
#define ATTEMPT_MAX     1024U

/* Define functions. */

/**
 * Initialise generators from the "playground.h" header.
 *
 * @see saved_nn_integer
 * @see saved_number
 * @see saved_comb_function
 * @see saved_len_generator
 *
 */
void initialise_generators (void)
{
    saved_nn_integer(3U);
    saved_number(
        0.0833333333333333333333333333333333333333333333333333333333333333
    );
    saved_comb_function(rmin);
    saved_len_generator(constant_length);
}

/**
 * Print an array of real values.
 *
 * @param out
 *     Buffer for the output.
 *
 * @param a
 *     Array of real values.
 *
 * @param n
 *     Length of the array `a` (number of elements).
 *
 */
void print_array (FILE* out, const real_t* a, size_t n)
{
    /* DECLARATION OF STATIC VARIABLES */

    /* Precision of the output (number of decimal places to print). */
    static const size_t prec = 8U;

    /* DECLARATION OF VARIABLES */

    /* Iteration index. */
    size_t i;

    /* INITIALISATION OF VARIABLES */

    /* Iteration index. */
    i = 0U;

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If either of the pointers `out` and `a` is a null-pointer, set the
         * number `n` to 0. */
        if (!(out && a))
        {
            /* Set the number `n` to 0. */
            n = 0U;

            /* Break the `do while`-loop. */
            break;
        }

        /* If the number of elements is 0, break the `do while`-loop. */
        if (!n)
            break;

        /* Print the elements. */
        fprintf(out, "%.*f", (int)prec, *a);
        for (i = 1U; i < n; ++i)
            fprintf(out, "\n%.*f", (int)prec, *(a + i));
    }
    while (false);
}

/**
 * The main program.
 *
 * @param argc
 *     Number of arguments given via the command line.
 *
 * @param argv
 *     Arguments given via the command line.
 *
 * @return
 *     Value 0 if the given arguments were correct and no memory allocation
 *     failed; otherwise a value other than 0.
 *
 */
int main (int argc, char** argv)
{
    /* CONSTANTS */

    /* Numerical approximation of the mathematical constant pi. */
    const real_t pi =
        3.1415926535897932384626433832795028841971693993751058209749445923;

    /* DECLARATION OF VARIABLES */

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

    /* Array of singular values. */
    real_t* s;

    /* Information from the SVD driver. */
    int info;

    /* Iteration index. */
    size_t i;

    /* INITIALISATION OF VARIABLES */

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

    /* Array of singular values. */
    s = (real_t*)(NULL);

    /* Information from the SVD driver. */
    info = 0;

    /* Iteration index. */
    i = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* Initialise the generating functions. */
    initialise_generators();

    /* Set the number of angles to N_ANGLES. */
    n = N_ANGLES;

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Allocate the memory for the polygons. */
    P = (real_t*)malloc((n << 1U) * sizeof *P);

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
    memset(P, 0, (n << 1U) * sizeof *P);

    /* Allocate memory for the arrays of the differences in coordinates, edges'
     * lengths and the outer angles. */
    dx = (real_t*)malloc(n * sizeof *dx);
    dy = (real_t*)malloc(n * sizeof *dy);
    l = (real_t*)malloc(n * sizeof *l);
    phi = (real_t*)malloc(n * sizeof *phi);

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
    memset(dx, 0, n * sizeof *dx);
    memset(dy, 0, n * sizeof *dy);
    memset(l, 0, n * sizeof *l);
    memset(phi, 0, n * sizeof *phi);

    /* Allocate memory for the array of singular values. */
    s = (real_t*)malloc(n * sizeof *s);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!s)
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Clear the memory in the arrays of the differences in coordinates,
         * edges' lengths and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

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
        memset(P, 0, (n << 1U) * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the array of singular values. */
    memset(s, 0, n * sizeof *s);

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
            random_coordinate,
            OUT_ITER_MAX,
            IN_ITER_MAX
        );

        /* Reorder the vertices by flipping the array from index 2 to the
         * end. */
        flip(P + 2U, n - 1U, 2U * sizeof *P);

        /* Simplify and check the array for the polygon.  If the array
         * represents a true `n`-gon, break the `for`-loop. */
        if (simplify_check_polygon(&n_true, P))
            if (n_true == n)
                break;

        /* Clear the memory in the array of points. */
        memset(P, 0, (n << 1U) * sizeof *P);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the original polygon has not been successfully generated, inform the
     * user and exit with a zero value. */
    if (i >= (ATTEMPT_MAX))
    {
        /* Inform the user. */
        printf("No true %lu-gon found.\n", n);

        /* Clear the memory in the array of singular values. */
        memset(s, 0, n * sizeof *s);

        /* Deallocate the memory allocated for the array of singular values. */
        free(s);
        s = (real_t*)(NULL);

        /* Clear the memory in the arrays of the differences in coordinates,
         * edges' lengths and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

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
        memset(P, 0, (n << 1U) * sizeof *P);

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

    /* Normalise the outer angles by dividing them by pi. */
    for (i = 0U; i < n; ++i)
        *(phi + i) /= pi;

    /* Display the polygon. */
    display(n, P, 1U);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Dump the polygon to `stdout`. */
    dump_polygons(stdout, n, P, 1U);
    printf("\n");

    /* Compute the singular values of the lengths of edges of the polygon. */
    s = svd_polygon(n, l, s, (real_t*)(NULL), &info);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If an error occured while computing the singular values, inform the user
     * and exit with a zero value. */
    if (info)
    {
        /* Inform the user. */
        printf("SVD driver responded with %d.", info);

        /* Clear the memory in the array of singular values. */
        memset(s, 0, n * sizeof *s);

        /* Deallocate the memory allocated for the array of singular values. */
        free(s);
        s = (real_t*)(NULL);

        /* Clear the memory in the arrays of the differences in coordinates,
         * edges' lengths and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

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
        memset(P, 0, (n << 1U) * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a zero value. */
        exit(EXIT_SUCCESS);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Print the singular values. */
    print_array(stdout, s, n);
    printf("\n");

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Print a blank line. */
    printf("\n");

    /* Compute the singular values of the outer angles of the polygon. */
    s = svd_polygon(n, phi, s, (real_t*)(NULL), &info);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If an error occured while computing the singular values, inform the user
     * and exit with a zero value. */
    if (info)
    {
        /* Inform the user. */
        printf("SVD driver responded with %d.", info);

        /* Clear the memory in the array of singular values. */
        memset(s, 0, n * sizeof *s);

        /* Deallocate the memory allocated for the array of singular values. */
        free(s);
        s = (real_t*)(NULL);

        /* Clear the memory in the arrays of the differences in coordinates,
         * edges' lengths and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

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
        memset(P, 0, (n << 1U) * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a zero value. */
        exit(EXIT_SUCCESS);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Print the singular values. */
    print_array(stdout, s, n);
    printf("\n");

    /* Clear the memory in the array of singular values. */
    memset(s, 0, n * sizeof *s);

    /* Deallocate the memory allocated for the array of singular values. */
    free(s);
    s = (real_t*)(NULL);

    /* Clear the memory in the arrays of the differences in coordinates,
     * edges' lengths and the outer angles. */
    memset(dx, 0, n * sizeof *dx);
    memset(dy, 0, n * sizeof *dy);
    memset(l, 0, n * sizeof *l);
    memset(phi, 0, n * sizeof *phi);

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
    memset(P, 0, (n << 1U) * sizeof *P);

    /* Deallocate the memory allocated for the array of points. */
    free(P);
    P = (real_t*)(NULL);

    /* Exit with a zero value. */
    return EXIT_SUCCESS;
}
