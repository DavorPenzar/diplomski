/**
 * Program for describing polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./describe in N n out
 * where:
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
 *     N   is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     out is the path to the output file to print the lengths of edges and the
 *         outer angles.
 *
 * Each polygon must be formated in the input file as
 *     x_0	y_0	x_1	y_1	...	x_n_minus_1	y_n_minus_1
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.  Whitespaces may differ (they may even be
 * spaces, tabs, line breaks...).  It is believed that each input polygon
 * truly represents a polygon of n vertices with vertices enumerated
 * positively---this is not checked and if any input polygon does not satisfy
 * this, results may be unexpected.
 *
 * Note that the input file must contain at least N polygons.  If, however, it
 * contains more than N polygons, only the first N polygons are read and the
 * information is computed on them.
 *
 * Each polygon is described in the output file in its own line formmated as
 *     l_0	l_1	...	l_n_minus_one	phi_1	phi_2	...	phi_n_minus_1	phi_0
 * where l_i denotes the length of the edge from the i-th vertex to the
 * (i + 1)-th vertex and phi_i denotes the outer angle at the i-th vertex.  The
 * outer angles are not normalised by dividing with pi.
 *
 * The pogram prints to the console the time elapsed only during the computation
 * of the information.  Time needed to read and print is not measured.
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
#include "playground.h"

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

    /* Error message for failing to read a coordinate. */
    const char* const err_msg_rc = "Reading a coordinate failed.";

    /* Format string for printing error messages. */
    const char* const format_err_msg = "%s\n";

    /* Mode of the input file to open. */
    const char* const file_in_open_mode = "rt";

    /* Mode of the output file to open. */
    const char* const file_out_open_mode = "wt";

    /* Format string for reading the coordinates. */
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

    /* Array of vertices. */
    real_t* P;

    /* Arrays of the differences in coordinates, the lengths of edges and the
     * outer angles. */
    real_t* dx;
    real_t* dy;
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

    /* Array of vertices. */
    P = (real_t*)(NULL);

    /* Arrays of the differences in coordinates, the lengths of edges and the
     * outer angles. */
    dx = (real_t*)(NULL);
    dy = (real_t*)(NULL);
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

    /* Allocate memory for the polygons. */
    P = (real_t*)malloc(((N * n) << 1U) * sizeof *P);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!P)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_mem);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise coordinates of vertices of polygons to zeros. */
    memset(P, 0, ((N * n) << 1U) * sizeof *P);

    /* Allocate memory for the arrays of the differences in coordinates, the
     * lengths of edges and the outer angles. */
    dx = (real_t*)malloc(((N * n) << 1U) * sizeof *dx);
    l = (real_t*)malloc(((N * n) << 1U) * sizeof *l);

    /* If the memory allocation has failed, deallocate memory, print the error
     * message and exit with a non-zero value. */
    if (!(dx && l))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_mem);

        /* Deallocate memory for the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the differences in coordinates, the lengths of edges and the
     * outer angles to zeros. */
    memset(dx, 0, ((N * n) << 1U) * sizeof *dx);
    memset(l, 0, ((N * n) << 1U) * sizeof *l);
    dy = dx + n;
    phi = l + n;

    /* Open the input file. */
    inout = fopen(*(argv + 1U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, ((N * n) << 1U) * sizeof *dx);
        memset(l, 0, ((N * n) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the input polygons. */
    for (i = 0; i < N; ++i)
    {
        /* Read the coordinates of the `i`-th input polygon.  If any of the
         * coordinates could not be read, print the error message, close the
         * input file, deallocate memory and exit with a non-zero value. */
        for (j = 0U; (j >> 1U) < n; ++j)
            if (!(fscanf(inout, format_input, P + ((i * n) << 1U) + j) == 1))
            {
                /* Print the error message. */
                fprintf(stderr, format_err_msg, err_msg_rc);

                /* Close the input file. */
                fclose(inout);
                inout = (FILE*)(NULL);

                /* Clear the memory in the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                memset(dx, 0, ((N * n) << 1U) * sizeof *dx);
                memset(l, 0, ((N * n) << 1U) * sizeof *l);

                /* Deallocate memory for the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                free(dx);
                dx = (real_t*)(NULL);
                dy = (real_t*)(NULL);
                free(l);
                l = (real_t*)(NULL);
                phi = (real_t*)(NULL);

                /* Clear the memory in the array of points. */
                memset(P, 0, ((N * n) << 1U) * sizeof *P);

                /* Deallocate memory for the polygons. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }
    }

    /* Close the input file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Get the current clock ticks. */
    t0 = clock();

    /* Describe all polygons. */
    for (i = 0U; i < N; ++i)
        describe_polygon(
            n,
            P + ((i * n) << 1U),
            dx + ((i * n) << 1U),
            dy + ((i * n) << 1U),
            l + ((i * n) << 1U),
            phi + ((i * n) << 1U)
        );

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

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, ((N * n) << 1U) * sizeof *dx);
        memset(l, 0, ((N * n) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the differences in
         * coordinates, the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Dump the information about polygons to the output file.  Note that all
     * polygons have exactly 2 * `n` bits of information stored in the array
     * `l`, so the `dump_polygons` function can be used to dump the
     * information. */
    dump_polygons(inout, n, l, N);

    /* Close the output file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Clear the memory in the arrays of the differences in coordinates,
     * the lengths of edges and the outer angles. */
    memset(dx, 0, ((N * n) << 1U) * sizeof *dx);
    memset(l, 0, ((N * n) << 1U) * sizeof *l);

    /* Deallocate memory for the arrays of the differences in
     * coordinates, the lengths of edges and the outer angles. */
    free(dx);
    dx = (real_t*)(NULL);
    dy = (real_t*)(NULL);
    free(l);
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, ((N * n) << 1U) * sizeof *P);

    /* Deallocate memory for the polygons. */
    free(P);
    P = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
