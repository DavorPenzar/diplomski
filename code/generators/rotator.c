/**
 * Program for generating polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./rotate N0 n in N1 out
 * where:
 *     N0  is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
 *     N1  is the number of polygons to generate from each read polygon (at
 *         least 1),
 *     out is the path to the output file to print the coordinates of vertices.
 *
 * Each polygon must be formated in the input file as
 *     x_0 y_0 x_1 y_1 ... x_n_minus_1 y_n_minus_1
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.  Whitespaces may differ (they may even be
 * spaces, tabs, line breaks...).  It is believed that each input polygon
 * truly represents a polygon of n vertices---this is not checked and if any
 * input polygon does not satisfy this, results may be unexpected.
 *
 * Note that the input file must contain at least N0 polygons.  If, however, it
 * contains more than N0 polygons, only the first N0 polygons are read and used
 * to generate new polygons.
 *
 * Suppose the i-th polygon is read from the input file (0 <= i < N0).  The
 * first polygon generated from the i-th polygon is the i-th polygon itself
 * (not even transposed, rotated, reflexed and/or scaled).  Each consecutive
 * polygon generated from the i-th polygon is generated by rotating the i-th
 * polygon by a random angle from the interval [0, 2 * pi).  Only after
 * generating all the N1 - 1 polygons from the i-th polygon is the original
 * polygon normalised to be of diameter 1 (up to a numerical precision).
 *
 * Each polygon is printed to the output file in its own line.  Each polygon is
 * formatted as
 *     x_0	y_0	x_1	y_1	...	x_n_minus_1	y_n_minus_1
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.  It is guaranteed that the polygons are of
 * diameter 1 (up to a numerical precision) and that they fit in the
 * [-1 / 2, 1 / 2] x [-1 / 2, 1 / 2] square in the plane.  The polygons are
 * printed in the order of reading and generating.
 *
 * Caution: the input and the output files are read and printed to
 * simultaneously to consume less memory.  Do not set the same path for the
 * input and the output file.
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

    /* Numerical approximation of double the mathematical constant pi (we SHALL
     * NOT call this "tau"). */
    const real_t pi2 =
        6.2831853071795864769252867665590057683943387987502116419498891846;

    /* Error message for an unknown environment error. */
    const char* const err_msg_env = "Unknown environment error.";

    /* Error message for the illegal number of additional arguments. */
    const char* const err_msg_argc =
        "Number of additional arguments must be 5: number of polygons to read, "
            "number of vertices, input file path, number of polygons to "
            "generate and output file path.";

    /* Error message for the illegal number of polygons to read. */
    const char* const err_msg_npr =
        "Number of polygons to read must be at least 1.";

    /* Error message for the illegal number of vertices. */
    const char* const err_msg_nv = "Number of vertices must be at least 3.";

    /* Error message for the illegal number of polygons to generate. */
    const char* const err_msg_npg =
        "Number of polygons to generate must be at least 1.";

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

    /* DECLARATION OF VARIABLES */

    /* Number of polygons to read. */
    size_t N0;

    /* Number of polygons to generate. */
    size_t N1;

    /* Number of vertices. */
    size_t n;

    /* Array of vertices. */
    real_t* P;

    /* Input file. */
    FILE* in;

    /* Output file. */
    FILE* out;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Number of polygons to read. */
    N0 = 0U;

    /* Number of polygons to generate. */
    N1 = 0U;

    /* Number of vertices. */
    n = 0U;

    /* Array of vertices. */
    P = (real_t*)(NULL);

    /* Input file. */
    in = (FILE*)(NULL);

    /* Output file. */
    out = (FILE*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* If the number of additional command line arguments is not 5, print the
     * error message and exit with a non-zero value. */
    if (!(argc == 6))
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

    /* If any of the 6 command line arguments is a null-pointer, print the error
     * message and exit with a non-zero value. */
    if (
        !(
            *argv &&
            *(argv + 1U) &&
            *(argv + 2U) &&
            *(argv + 3U) &&
            *(argv + 4U) &&
            *(argv + 5U)
        )
    )
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_env);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Scan the number of polygons to read. */
    N0 = (size_t)atoi(*(argv + 1U));

    /* Scan the number of vertices. */
    n = (size_t)atoi(*(argv + 2U));

    /* Scan the number of polygons to generate. */
    N1 = (size_t)atoi(*(argv + 4U));

    /* If the number of polygons to read is 0, print the error message and exit
     * with a non-zero value. */
    if (!N0)
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

    /* If the number of polygons to generate is 0, print the error message and
     * exit with a non-zero value. */
    if (!N1)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_npg);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the polygons. */
    P = (real_t*)malloc(((N1 * n) << 1U) * sizeof *P);

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
    memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

    /* Open the input file. */
    in = fopen(*(argv + 3U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!in)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Open the output file. */
    out = fopen(*(argv + 5U), file_out_open_mode);

    /* If the output file could not be opened, print the error message,
     * close the input file, deallocate memory and exit with a non-zero
     * value. */
    if (!out)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_out);

        /* Close the input file. */
        fclose(in);
        in = (FILE*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read and generate polygons. */
    for (i = 0U; i < N0; ++i)
    {
        /* Clear the memory in the array of points. */
        memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

        /* Read the coordinates of the `i`-th input polygon.  If any of the
         * coordinates could not be read, print the error message, close the
         * output and the input files, deallocate memory and exit with a
         * non-zero value. */
        for (j = 0U; (j >> 1U) < n; ++j)
            if (!(fscanf(in, format_input, P + j) == 1))
            {
                /* Print the error message. */
                fprintf(stderr, format_err_msg, err_msg_rc);

                /* Close the output file. */
                fclose(out);
                out = (FILE*)(NULL);

                /* Close the input file. */
                fclose(in);
                in = (FILE*)(NULL);

                /* Clear the memory in the array of points. */
                memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

                /* Deallocate memory for the polygons. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

        /* Generate the rest `N1` - 1 polygons. */
        for (j = 1U; j < N1; ++j)
        {
            /* Copy the original polygon. */
            memcpy(P + ((j * n) << 1U), P, (n << 1U) * sizeof *P);

            /* Rotate the polygon by a random angle. */
            rotate_polygon(n, P + ((j * n) << 1U), pi2 * rrand());

            /* Normalise the `j`-th polygon. */
            normalise_polygon(n, P);
        }

        /* Normalise the first polygon. */
        normalise_polygon(n, P);

        /* Dump the polygons to the output file. */
        dump_polygons(out, n, P, N1);
    }

    /* Close the output file. */
    fclose(out);
    out = (FILE*)(NULL);

    /* Close the input file. */
    fclose(in);
    in = (FILE*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

    /* Deallocate memory for the polygons. */
    free(P);
    P = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
