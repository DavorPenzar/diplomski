/**
 * Program for generating and displaying polygons.
 *
 * Usage:
 *     ./generate n
 * where "./generate" is the command of the compiled program and n is the
 * decimal representation of the number of vertices (>= 3).
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
#include "array.h"
#include "boolean.h"
#include "numeric.h"
#include "polygon.h"
#include "playground.h"

/* Define constants for the numerical approximation of the mathematical constant
 * pi and some of its fractions. */
#define PI      3.1415926535897932384626433832795028841971693993751058209749445923L
#define PI_2    1.5707963267948966192313216916397514420985846996875529104874722962L
#define PI_3    1.0471975511965977461542144610931676280657231331250352736583148641L
#define PI_4    0.7853981633974483096156608458198757210492923498437764552437361481L
#define PI_5    0.7853981633974483096156608458198757210492923498437764552437361481L
#define PI_6    0.5235987755982988730771072305465838140328615665625176368291574321L
#define PI_7    0.4487989505128276054946633404685004120281670570535865458535635132L
#define PI_8    0.3926990816987241548078304229099378605246461749218882276218680740L
#define PI_9    0.3490658503988659153847381536977225426885743777083450912194382880L
#define PI_10   0.3141592653589793238462643383279502884197169399375105820974944592L
#define PI_11   0.2855993321445266580420584893890457167451972181250096200886313266L
#define PI_12   0.2617993877991494365385536152732919070164307832812588184145787160L

/* Define constants for converting radians to degrees and vice versa. */
#define DEG_TO_RAD  57.295779513082320876798154814105170332405472466564321549160243861L
#define RAD_TO_DEG   0.017453292519943295769236907684886127134428718885417254560971914L

/* Define constants for maximal numbers of iterations. */
#define IN_ITER_MAX     1024U
#define OUT_ITER_MAX    1024U
#define ATTEMPT_MAX     1024U

/* Define functions. */

#if !defined(__cplusplus)
void initialise_generators (void)
#else
void initialise_generators ()
#endif /* __cplusplus */
{
    saved_number(1.0);
    saved_comb_function(rmin);
    saved_len_generator(combiner_length);
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
    P = (real_t*)malloc(2U * N * n * sizeof *P);

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
    memset(P, 0, 2U * N * n * sizeof *P);

    /* Allocate memory for the arrays of the edges' lengths and the outer
     * angles. */
    l = (real_t*)malloc(N * n * sizeof *l);
    phi = (real_t*)malloc(N * n * sizeof *phi);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!(l && phi))
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Deallocate the memory allocated for the arrays of the edges' lengths
         * and the outer angles. */
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

    /* Initialise the arrays of the edges' lengths and the outer engles to
     * zeros. */
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *phi);

    /* Save values and pointers to arrays for generating new values. */
    saved_nn_integer(n);
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
            scan_coordinate,
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
        memset(P, 0, 2U * n * sizeof *P);
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

        /* Clear the memory in the array of points. */
        memset(P, 0, 2U * N * n * sizeof *P);

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
    describe_polygon(n, P, l, phi);

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
            random_polygon(n, P + 2U * j * n, perturbate_coordinate);

            /* Reorder the vertices by flipping the array from index 2 to the
             * end. */
            flip(P + 2U * j * n + 2U, n - 1U, 2U * sizeof *P);

            /* Simplify and check the array for the polygon.  If the array
             * represents a true `n`-gon, break the `for`-loop. */
            if (simplify_check_polygon(&n_true, P + 2U * j * n))
                if (n_true == n)
                    break;

            /* Clear the memory in the array of points. */
            memset(P + 2U * j * n, 0, 2U * n * sizeof *P);
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

            /* Clear the memory in the arrays of the edges' lengths and the
             * outer angles. */
            memset(l, 0, N * n * sizeof *l);
            memset(phi, 0, N * n * sizeof *phi);

            /* Deallocate the memory allocated for the arrays of the edges'
             * lengths and the outer angles. */
            free(l);
            l = (real_t*)(NULL);
            free(phi);
            phi = (real_t*)(NULL);

            /* Clear the memory in the array of points. */
            memset(P, 0, 2U * N * n * sizeof *P);

            /* Deallocate the memory allocated for the array of points. */
            free(P);
            P = (real_t*)(NULL);

            /* Exit with a zero value. */
            exit(EXIT_SUCCESS);
        }

        /* Normalise the perturbated polygon. */
        normalise_polygon(n, P + 2U * j * n);

        /* Compute the edges' lengths and the outer angles of the original
         * polygon. */
        describe_polygon(n, P + 2U * j * n, l + j * n, phi + j * n);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Display the polygons and print the returned value. */
    return_value = display(n, P, N);
    printf("Returned: %d (0x%03X)\n", return_value, return_value);

    /* Clear the memory in the arrays of the edges' lengths and the outer
     * angles. */
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *l);

    /* Deallocate the memory allocated for the arrays of the edges' lengths and
     * the outer angles. */
    free(l);
    l = (real_t*)(NULL);
    free(phi);
    phi = (real_t*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, 2U * j * n * sizeof *P);

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
