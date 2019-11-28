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
#include "polygon.h"
#include "boolean.h"
#include "numeric.h"

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

/**
 * Whether or not the polygon path should be printed.
 *
 */
const   bool    print_polygon_path  =   true;

/* Define functions. */

size_t saved_number (const size_t* n)
{
    static size_t saved_n = 0U;

    if (n)
        saved_n = *n;

    return saved_n;
}

const real_t* saved_polygon (const real_t* P)
{
    static const real_t* saved_P = (real_t*)(NULL);

    if (P)
        saved_P = P;

    return saved_P;
}

const real_t* saved_lengths (const real_t* l)
{
    static const real_t* saved_l = (real_t*)(NULL);

    if (l)
        saved_l = l;

    return saved_l;
}

const real_t* saved_angles (const real_t* phi)
{
    static const real_t* saved_phi = (real_t*)(NULL);

    if (phi)
        saved_phi = phi;

    return saved_phi;
}

real_t perturbate (size_t i, size_t coordinate)
{
    static real_t r = 0.0;
    static real_t phi = 0.0;

    const real_t* P;
    const real_t* l;

    real_t return_value;

    P = (real_t*)(NULL);

    return_value = 0.0;

    if (
        i < saved_number((size_t*)(NULL)) &&
        (coordinate == 0U || coordinate == 1U)
    )
    {
        l = saved_lengths((real_t*)(NULL));
        P = saved_polygon((real_t*)(NULL));

        if (coordinate == 0U)
        {
            r = (
                rrandn() *
                rmin(*(l + decmod(i, saved_number((size_t*)(NULL)))), *(l + i))
            );
            phi = (real_t)((long double)rrand() * (PI));
        }

        return_value = *(P + 2U * i + coordinate);

        return_value += (
            (coordinate == 0U) ?
                (r * rcos(phi)) :
                ((coordinate == 1U) ? (r * rsin(phi)) : 0.0)
        );
    }

    return return_value;
}

/**
 * Generate a pseudorandom coordinate from the interval [0, 1].
 *
 * The function is actually only a wrapper around the `rrand` function.  The
 * purpose of the function is to pass it as the argument to the `random_polygon`
 * and the `smart smart_random_polygon` functions.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     Value returned by calling `rrand()`.
 *
 */
real_t random_coordinate (size_t i, size_t coordinate)
{
    return rrand();
}

/**
 * Normalise a polygon.
 *
 * The function is the wrapper around the functions
 * `correct_polygon_orientation`, `centralise_polygon` and
 * `standardise_polygon`.
 *
 * The given array is mutated in the functions.
 *
 * @param n
 *     Number of points.
 *
 * @param P
 *     Array of points of size at least 2 * `n`.  The array is organised as
 *     `{x_0, y_0, x_1, y_1, ..., x_n_minus_1, y_n_minus_1}`, where `x_i` is the
 *     x-coordinate of the `i`-th point and `y_i` is its y-coordinate.  Note
 *     that the first and the last points are neighbouring.
 *
 *     Caution: the array `P` is mutated in the functions.
 *
 */
void normalise_polygon (size_t n, real_t* P)
{
    /* If the pointer `P` is a null-pointer, set the number `n` to 0. */
    if (!P)
        n = 0U;

    /* Correct the order of the points. */
    correct_polygon_orientation(n, P);

    /* Centralise the polygon. */
    centralise_polygon(n, P);

    /* Standardise the polygon. */
    standardise_polygon(n, P);
}

/**
 * Display polygons that fit inside the square [-1, 1] x [-1, 1].
 *
 * The polygons are displayed by calling the application "display.py".
 *
 * The given array is not mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the command
 * used to call the application for displaying.  If that happens, the returned
 * value may be unexpected.  Exceptions thrown by failed memory allocation in
 * C++ are not caught.
 *
 * @param n
 *     Number of vertices of each polygon.
 *
 * @param P
 *     Array of points of size at least 2 * `n` * `N`.  Each segment of length
 *     2 * `n` of the array (from indices 2 * i including to 2 * (i + 1)
 *     excluding for an index 0 <= i < N) represents a polygon and is organised
 *     as
 *     `{x_0, y_0, x_1, y_1, ..., x_n_minus_1, y_n_minus_1}`, where `x_i` is the
 *     x-coordinate of the `i`-th vertex of the polygon and `y_i` is its
 *     y-coordinate.  Note that the first and the last points inside a segment
 *     are neighbouring vertices of the polygon.
 *
 * @param N
 *     Number of polygons.
 *
 * @return
 *     The value returned by the application used to display the polygons.
 *
 */
int display (size_t n, const real_t* P, size_t N)
{
    /* The base command for the application used to display the polygon. */
    static const char base_command[13U] = "./display.py";

    /* The length of the base command. */
    static const size_t base_length = 12U;

    /* DECLARATION OF VARIABLES */

    /* Value returned by the application. */
    int return_value;

    /* Command. */
    char* command;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Value returned by the application. */
    return_value = 0;

    /* Command. */
    command = (char*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the number of polygons is 0, break the `do while`-loop. */
        if (!N)
            break;

        /* Allocate the memory for the command. */
        command = (char*)malloc(
            (base_length + N * 16U * n + (N - 1U) * 2U) * sizeof command
        );

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!command)
            break;

        /* Initialise the command to zeros. */
        memset(command, 0, (19U + 16U * n) * sizeof *command);

        /* Print the base command to the beginning of the command. */
        sprintf(command, "%s", base_command);

        /* Iterate over the arrays and print the coordinates to the command. */
        for (j = 0U; j / 2U < n; ++j)
            sprintf(command + base_length + 8U * j, " %7.4f", *(P + j));
        for (i = 1U; i < N; ++i)
        {
            sprintf(command + base_length + 16U * i * n + 2U * (i - 1U), " +");
            for (j = 0U; j / 2U < n; ++j)
                sprintf(
                    command + base_length + i * (16U * n + 2U) + 8U * j,
                    " %7.4f",
                        *(P + 2U * i * n + j)
                );
        }

        /* Call the command and get the returned value. */
        return_value = system(command);

        /* Clear the memory in the command. */
        memset(command, 0, (19U + 16U * n) * sizeof *command);

        /* Deallocate the memory allocated for the command. */
        free(command);
        command = (char*)(NULL);
    }
    while (false);

    /* Return the returned value. */
    return return_value;
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
        printf("No true %lu-gon found.", n);

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

    /* Save the number of vertices, array of points and arrays of edges' lengths
     * and outer edges. */
    saved_number(&n);
    saved_polygon(P);
    saved_lengths(l);
    saved_angles(phi);

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
            random_polygon(n, P + 2U * j * n, perturbate);

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
