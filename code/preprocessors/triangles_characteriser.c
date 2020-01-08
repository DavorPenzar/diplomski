/**
 * Program for describing polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./describe in N out
 * where:
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
 *     N   is the number of triangles to read (at least 1),
 *     out is the path to the output file to print the characteristic points.
 *
 * Each triangle must be formated in the input file as
 *     l_0	l_1	l_2	phi_1	phi_2	phi_0
 * where l_i denotes the length of the edge from the i-th vertex to the
 * (i + 1)-th vertex and phi_i denotes the outer angle at the i-th vertex not
 * normalised by dividing with pi.  It is believed that each input triangle
 * truly represents a triangle ---this is not checked and if any input triangle
 * does not satisfy this, results may be unexpected.
 *
 * Note that the input file must contain at least N triangles.  If, however, it
 * contains more than N triangles, only the first N triangles are read and
 * characterised.
 *
 * Each triangle is characterised in the output file in its own line formmated
 * as
 *     x	y
 * where (x, y) are the coordinates of the characteristic point of the triangle.
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
#include "triangle.h"

int main (int argc, char** argv)
{
    /* DECLARATION OF CONSTANTS */

    /* Number of clock ticks per second as `double`. */
    const double clocks_per_sec = (double)(CLOCKS_PER_SEC);

    /* Error message for an unknown environment error. */
    const char* const err_msg_env = "Unknown environment error.";

    /* Error message for the illegal number of additional arguments. */
    const char* const err_msg_argc =
        "Number of additional arguments must be 3: input file path, number of "
            "triangles to read and output file path.";

    /* Error message for the illegal number of triangles to read. */
    const char* const err_msg_npr =
        "Number of triangles to read must be at least 1.";

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

    /* Arrays of the lengths of edges and the outer angles. */
    real_t* l;
    real_t* phi;

    /* Array of characteristic points of triangles. */
    real_t* C;

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

    /* Arrays of the lengths of edges and the outer angles. */
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Array of characteristic points of triangles. */
    C = (real_t*)(NULL);

    /* Input/output file. */
    inout = (FILE*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* If the number of additional command line arguments is not 3, print the
     * error message and exit with a non-zero value. */
    if (!(argc == 4))
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

    /* If any of the 4 command line arguments is a null-pointer, print the error
     * message and exit with a non-zero value. */
    if (
        !(
            *argv &&
            *(argv + 1U) &&
            *(argv + 2U) &&
            *(argv + 3U)
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

    /* If the number of polygons to read is 0, print the error message and exit
     * with a non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_npr);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the arrays of the lengths of edges and the outer
     * angles. */
    l = (real_t*)malloc(((3U * N) << 1U) * sizeof *l);

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
    memset(l, 0, ((3U * N) << 1U) * sizeof *l);
    phi = l + n;

    /* Allocate memory for the array of characteristic points of triangles. */
    C = (real_t*)malloc((N << 1U) * sizeof *C);

    /* If the memory allocation has failed, deallocate memory, print the error
     * message and exit with a non-zero value. */
    if (!C)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the lengths of edges and the outer
         * angles. */
        memset(l, 0, ((3U * N) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the lengths of edges and the
         * outer angles. */
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise coordinates of characteristic points of triangles to zeros. */
    memset(C, 0, (N << 1U) * sizeof *C);

    /* Open the input file. */
    inout = fopen(*(argv + 1U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the array of characteristic points of
         * triangles. */
        memset(C, 0, (N << 1U) * sizeof *C);

        /* Deallocate memory for the array of characteristic points of
         * triangles. */
        free(C);
        C = (real_t*)(NULL);

        /* Clear the memory in the arrays of the lengths of edges and the outer
         * angles. */
        memset(l, 0, ((3U * N) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the lengths of edges and the
         * outer angles. */
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the input triangles. */
    for (i = 0; i < N; ++i)
    {
        /* Read the lengths of edges and the outer angles of the `i`-th input
         * triangle.  If any of the coordinates could not be read, print the
         * error message, close the input file, deallocate memory and exit with
         * a non-zero value. */
        for (j = 0U; (j >> 1U) < 3U; ++j)
            if (!(fscanf(inout, format_input, l + ((i * n) << 1U) + j) == 1))
            {
                /* Print the error message. */
                fprintf(stderr, format_err_msg, err_msg_rn);

                /* Close the input file. */
                fclose(inout);
                inout = (FILE*)(NULL);

                /* Clear the memory in the array of characteristic points of
                 * triangles. */
                memset(C, 0, (N << 1U) * sizeof *C);

                /* Deallocate memory for the array of characteristic points of
                 * triangles. */
                free(C);
                C = (real_t*)(NULL);

                /* Clear the memory in the arrays of the lengths of edges and
                 * the outer angles. */
                memset(l, 0, ((3U * N) << 1U) * sizeof *l);

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

    /* Characterise all triangles. */
    for (i = 0U; i < N; ++i)
        char_triangle(
            l + ((3U * i) << 1U),
            phi + ((3U * i) << 1U),
            C + (i << 1U),
            C + (i << 1U) + 1U,
            true
        );

    /* Get the current clock ticks. */
    t1 = clock();

    /* Print the time elapsed during the description of polygons. */
    printf(format_time, (double)(t1 - t0) / clocks_per_sec);

    /* Open the output file. */
    inout = fopen(*(argv + 3U), file_out_open_mode);

    /* If the output file could not be opened, print the error message,
     * close the input file, deallocate memory and exit with a non-zero
     * value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_out);

        /* Clear the memory in the array of characteristic points of
         * triangles. */
        memset(C, 0, (N << 1U) * sizeof *C);

        /* Deallocate memory for the array of characteristic points of
         * triangles. */
        free(C);
        C = (real_t*)(NULL);

        /* Clear the memory in the arrays of the lengths of edges and the outer
         * angles. */
        memset(l, 0, ((3U * N) << 1U) * sizeof *l);

        /* Deallocate memory for the arrays of the lengths of edges and the
         * outer angles. */
        free(l);
        l = (real_t*)(NULL);
        phi = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Dump the characteristic points of triangles.  Note that all triangles
     * have exactly 2 coordinates stored in the array `C`, so the
     * `dump_polygons` function can be used to dump the characteristic
     * points. */
    dump_polygons(inout, 1U, C, N);

    /* Close the output file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Clear the memory in the array of characteristic points of triangles. */
    memset(C, 0, (N << 1U) * sizeof *C);

    /* Deallocate memory for the array of characteristic points of triangles. */
    free(C);
    C = (real_t*)(NULL);

    /* Clear the memory in the arrays of the lengths of edges and the outer
     * angles. */
    memset(l, 0, ((3U * N) << 1U) * sizeof *l);

    /* Deallocate memory for the arrays of the lengths of edges and the outer
     * angles. */
    free(l);
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
