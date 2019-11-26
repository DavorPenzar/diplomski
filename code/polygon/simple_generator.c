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

/* Define the constant for the approximation of the mathematical constant pi. */
#define PI  3.141592653589793

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
 * Display a polygon that fits inside the square [-1, 1] x [-1, 1].
 *
 * The polygon is displayed by calling the application "display.py".
 *
 * The given array is not mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the command
 * used to call the application for displaying.  If that happens, the returned
 * value may be unexpected.  Exceptions thrown by failed memory allocation in
 * C++ are not caught.
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
 * @return
 *     The value returned by the application used to display the polygon.
 *
 */
int display (size_t n, const real_t* P)
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

    /* Iteration index. */
    size_t i;

    /* INITIALISATION OF VARIABLES */

    /* Value returned by the application. */
    return_value = 0;

    /* Command. */
    command = (char*)(NULL);

    /* Iteration index. */
    i = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* Allocate the memory for the command. */
        command = (char*)malloc((base_length + 16U * n) * sizeof command);

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!command)
            break;

        /* Initialise the command to zeros. */
        memset(command, 0, (19U + 16U * n) * sizeof *command);

        /* Print the base command to the beginning of the command. */
        sprintf(command, "%s", base_command);

        /* Iterate over the array and print the coordinates to the command. */
        for (i = 0U; i / 2U < n; ++i)
            sprintf(command + base_length + 8U * i, " %7.4f", *(P + i));

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
 *     Number of arguments given via the command line.  Should be 2.
 *
 * @param argv
 *     Arguments given via the command line.  Should be an array of length 2
 *     such that the first element is the name (command) of the program and the
 *     second element is the decimal representation of the number of vertices.
 *
 * @return
 *     Value 0 if the given arguments were correct; otherwise a value other than
 *     0.
 *
 */
int main (int argc, char** argv)
{
    /* DECLARATION OF VARIABLES */

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

    /* Iteration index. */
    size_t i;

    /* INITIALISATION OF VARIABLES */

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

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the number of command line arguments is not 2, print the error message
     * and exit with a non-zero value. */
    if (!(argc == 2))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "Exactly one additional command line argument (number of vertices) "
                "must be given.\n"
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
    if (!(*argv && *(argv + 1U)))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "At least one of the command line arguments is a null-pointer.\n"
        );

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the expected number of vertices from the second command line
     * argument. */
    n = (size_t)atoi(*(argv + 1U));

    /* If the number of vertices is strictly less than 3, print the error
     * message and exit with a non-zero value. */
    if (n < 3U)
    {
        /* Print the error message. */
        fprintf(stderr, "Number of vertices must be at least 3.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Attempt to generate an `n`-gon ATTEMPT_MAX times at the most. */
    for (i = 0U; i < (ATTEMPT_MAX); ++i)
    {
        /* Set the value of number `n_true` to the value of `n`. */
        n_true = n;

        /* Generate an array of `n` points using the `smart_random_polygon`
         * function. */
        P = smart_random_polygon(
            n,
            (real_t*)(NULL),
            random_coordinate,
            OUT_ITER_MAX,
            IN_ITER_MAX
        );

        /* Simplify and check the array for the polygon.  If the array
         * represents a polygon, break the `for`-loop. */
        if (simplify_check_polygon(&n_true, P))
            break;

        /* Clear the memory in the array of points if the pointer `P` is not a
         * null-pointer. */
        if (P)
            memset(P, 0, 2U * n * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the polygon has been successfully generated, display it. */
    if (i < ATTEMPT_MAX)
    {
        /* Normalise the polygon. */
        normalise_polygon(n_true, P);

        /* Inform the user about the number of attempts and the final number of
         * vertices. */
        printf(
            "Attempts: %lu\nFinal number of true vertices: %lu\n",
                i + 1U,
                n
        );

        /* If the polygon path should be printed, compute and print it. */
        if (print_polygon_path)
        {
            /* Allocate memory for the arrays of the edges' lengths and the
             * outer angles. */
            l = (real_t*)malloc(n_true * sizeof *l);
            phi = (real_t*)malloc(n_true * sizeof *phi);

            /* If the memory allocation has failed, print the error message,
            * deallocate memory and exit with a non-zero value. */
            if (!(l && phi))
            {
                /* Print the error message. */
                fprintf(stderr, "Memory allocation fail.\n");

                /* Deallocate the memory allocated for the arrays of the edges'
                * lengths and the outer angles. */
                free(l);
                l = (real_t*)(NULL);
                free(phi);
                phi = (real_t*)(NULL);

                /* Clear the memory in the array of points. */
                memset(P, 0, 2U * n * sizeof *P);

                /* Deallocate the memory allocated for the array of points. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

            /* Initialise the arrays of the edges' lengths and the outer engles
             * to zeros. */
            memset(l, 0, n_true * sizeof *l);
            memset(phi, 0, n_true * sizeof *phi);

            /* Compute the edges' lengths and the outer angles. */
            describe_polygon(n_true, P, l, phi);

            /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
            fflush(stdin);
            fflush(stdout);
            fflush(stderr);

            /* Print the path of the polygon. */
            printf("Polygon path:\n");
            for (i = 0U; i < n_true; ++i)
                printf("\t%6.4f @ %7.4f pi\n", *(l + i), *(phi + i) / (PI));

            /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
            fflush(stdin);
            fflush(stdout);
            fflush(stderr);

            /* Clear the memory in the arrays of the edges' lengths and the
             * outer angles. */
            memset(l, 0, n_true * sizeof *l);
            memset(phi, 0, n_true * sizeof *l);

            /* Deallocate the memory allocated for the arrays of the edges'
             * lengths and the outer angles. */
            free(l);
            l = (real_t*)(NULL);
            free(phi);
            phi = (real_t*)(NULL);
        }

        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);

        /* Display the polygon and print the returned value. */
        return_value = display(n_true, P);
        printf("Returned: %d (0x%03X)\n", return_value, return_value);

        /* Clear the memory in the array of points. */
        memset(P, 0, 2U * n * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);
    }
    /* Otherwise inform the user that no `n`-gon has been found. */
    else
        printf("No %lu-gon found.\n", n);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Return 0 after successfully running the program. */
    return EXIT_SUCCESS;
}
