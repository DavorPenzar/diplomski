/**
 * Program for sorting data of polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./sort in N n out
 * where:
 *     in  is the path to the input file to read the lengths of edges and the
 *         outer angles,
 *     N   is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     out is the path to the output file to print the singular values of the
 *         lengths of edges and the outer angles.
 *
 * Each polygon must be formated in the input file as
 *     l_0	l_1	...	l_n_minus_1	phi_1	phi_2	...	phi_n_minus_1	phi_0
 * where l_i denotes the length of the edge from the i-th vertex to the
 * (i + 1)-th vertex and phi_i denotes the outer angle at the i-th vertex not
 * normalised by dividing with pi.  It is believed that each input polygon
 * truly represents a polygon of n vertices ---this is not checked and if any
 * input polygon does not satisfy this, results may be unexpected.
 *
 * Note that the input file must contain at least N polygons.  If, however, it
 * contains more than N polygons, only the first N polygons are read and SVD is
 * done on them.
 *
 * Each polygon's information is printed in the input file as
 *     l_0^*	l_1^*	...	l_n_minus_1^*	phi_0^*	phi_1^*	phi_n_minus_1^*
 * where l_i^* the i-th largest (counting their multiplicities) length of edges
 * and phi_i^* is the i-th smallest (counting their multiplicities) outer angls
 * of the polygon.
 *
 * The pogram prints to the console the time elapsed only during sorting the
 * values.  Time needed to read and print is not measured.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Compile with mathematical constants from "math.h". */
#define _USE_MATH_DEFINES   1

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

int main (int argc, char** argv)
{
    /* DECLARATION OF CONSTANTS */

    /* Number of clock ticks per second as `double`. */
    const double clocks_per_sec = (double)(CLOCKS_PER_SEC);

    /* Error message for an unknown environment error. */
    const char* const err_msg_env = "Unknown environment error.";

    /* Error message for the illegal number of additional arguments. */
    const char* const err_msg_argc =
        "Number of additional arguments must be 4: input file path, number of "
            "polygons to read, number of vertices and output file path.";

    /* Error message for the illegal number of polygons to read. */
    const char* const err_msg_npr =
        "Number of polygons to read must be at least 1.";

    /* Error message for the illegal number of vertices. */
    const char* const err_msg_nv = "Number of vertices must be at least 3.";

    /* Error message for the memory allocation fail. */
    const char* const err_msg_mem = "Memory allocation fail.";

    /* Error message for input file opening fail. */
    const char* const err_msg_in = "Input file cannot be opened.";

    /* Error message for output file opening fail. */
    const char* const err_msg_out = "Output file cannot be opened.";

    /* Error message for failing to read a number. */
    const char* const err_msg_rn = "Reading a number failed.";

    /* Format string for printing error messages. */
    const char* const format_err_msg = "%s\n";

    /* Mode of the input file to open. */
    const char* const file_in_open_mode = "rt";

    /* Mode of the output file to open. */
    const char* const file_out_open_mode = "wt";

    /* Format string for reading the numbers. */
    const char* const format_input = " %lf";

    /* Format string for printing the time elapsed. */
    const char* const format_time = "Time elapsed: %.6f s.\n";

    /* DECLARATION OF VARIABLES */

    /* Clock ticks. */
    clock_t t0;
    clock_t t1;

    /* Number of polygons to read. */
    size_t N;

    /* Number of vertices. */
    size_t n;

    /* Arrays of the lengths of edges and the outer angles. */
    real_t* l;
    real_t* phi;

    /* Input/output file. */
    FILE* inout;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Clock ticks. */
    t0 = 0U;
    t1 = 0U;

    /* Number of polygons to read. */
    N = 0U;

    /* Number of vertices. */
    n = 0U;

    /* Arrays of the lengths of edges and the outer angles. */
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Input/output file. */
    inout = (FILE*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* If the number of additional command line arguments is not 4, print the
     * error message and exit with a non-zero value. */
    if (!(argc == 5))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_argc);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If `argv` is a null-pointer, print the error message and exit with a
     * non-zero value. */
    if (!argv)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_env);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If any of the 5 command line arguments is a null-pointer, print the error
     * message and exit with a non-zero value. */
    if (
        !(
            *argv &&
            *(argv + 1U) &&
            *(argv + 2U) &&
            *(argv + 3U) &&
            *(argv + 4U)
        )
    )
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_env);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Scan the number of polygons to read. */
    N = (size_t)atoi(*(argv + 2U));

    /* Scan the number of vertices. */
    n = (size_t)atoi(*(argv + 3U));

    /* If the number of polygons to read is 0, print the error message and exit
     * with a non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_npr);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the number of vertices is strictly less than 3, print the error
     * message and exit with a non-zero value. */
    if (n < 3U)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_nv);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the arrays of the lengths of edges and the outer
     * angles. */
    l = (real_t*)malloc(((N * n) << 1U) * sizeof *l);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!l)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_mem);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the lengths of edges and the outer angles to zeros. */
    memset(l, 0, ((N * n) << 1U) * sizeof *l);
    phi = l + n;

    /* Open the input file. */
    inout = fopen(*(argv + 1U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the lengths of edges and the outer
         * angles. */
        memset(l, 0, ((N * n) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the lengths of edges and the
         * outer angles. */
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the input polygons. */
    for (i = 0; i < N; ++i)
    {
        /* Read the lengths of edges and the outer angles of the `i`-th input
         * polygon.  If any of the coordinates could not be read, print the
         * error message, close the input file, deallocate memory and exit with
         * a non-zero value. */
        for (j = 0U; (j >> 1U) < n; ++j)
            if (!(fscanf(inout, format_input, l + ((i * n) << 1U) + j) == 1))
            {
                /* Print the error message. */
                fprintf(stderr, format_err_msg, err_msg_rn);

                /* Close the input file. */
                fclose(inout);
                inout = (FILE*)(NULL);

                /* Clear the memory in the arrays of the lengths of edges and
                 * the outer angles. */
                memset(l, 0, ((N * n) << 1U) * sizeof *l);

                /* Deallocate memory for the arrays of the lengths of edges and
                 * the outer angles. */
                free(l);
                l = (real_t*)(NULL);
                phi = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }
    }

    /* Close the input file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Get the current clock ticks. */
    t0 = clock();

    /* Compute the singular values. */
    for (i = 0U; i < N; ++i)
    {
        /* Sort the lengths of edges of the `i`-th polygon. */
        qsort(l + ((i * n) << 1U), n, sizeof *l, ricompar);

        /* Sort the outer angles of the `i`-th polygon. */
        qsort(phi + ((i * n) << 1U), n, sizeof *phi, rcompar);
    }

    /* Get the current clock ticks. */
    t1 = clock();

    /* Print the time elapsed during the description of polygons. */
    printf(format_time, (double)(t1 - t0) / clocks_per_sec);

    /* Open the output file. */
    inout = fopen(*(argv + 4U), file_out_open_mode);

    /* If the output file could not be opened, print the error message,
     * close the input file, deallocate memory and exit with a non-zero
     * value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_out);

        /* Clear the memory in the arrays of the lengths of edges and the outer
         * angles. */
        memset(l, 0, ((N * n) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the lengths of edges and the
         * outer angles. */
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Dump the sorted information about polygons to the output file.  Note that
     * all polygons have exactly 2 * `n` bits of information stored in the array
     * `l`, so the `dump_polygons` function can be used to dump the
     * information. */
    dump_polygons(inout, n, l, N);

    /* Close the output file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Clear the memory in the arrays of the lengths of edges and the outer
     * angles. */
    memset(l, 0, ((N * n) << 1U) * sizeof *l);

    /* Deallocate memory for the arrays of the lengths of edges and the outer
     * angles. */
    free(l);
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}