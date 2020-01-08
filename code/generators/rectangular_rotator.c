/**
 * Program for generating polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./rotate N n in out
 * where:
 *     N   is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
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
 * Note that the input file must contain at least N polygons.  If, however, it
 * contains more than N polygons, only the first N polygons are read and used to
 * generate new polygons.
 *
 * Suppose the i-th polygon is read from the input file (0 <= i < N0).  The
 * first polygon generated from the i-th polygon is the i-th polygon itself.
 * The next generated polygon is the i-th polygon rotated by pi / 2, then by pi
 * and then by 3 * pi / 2.
 *
 * Each polygon is printed to the output file in its own line.  Each polygon is
 * formatted as
 *     x_0	y_0	x_1	y_1	...	x_n_minus_1	y_n_minus_1
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.
 *
 * Caution: the input and the output files are read and printed to
 * simultaneously to consume memory.  Do not set the same path for the input
 * and the output file.
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

/* Define constants for maximal numbers of iterations. */
#define IN_ITER_MAX     1024U
#define OUT_ITER_MAX    1024U
#define ATTEMPT_MAX     1024U

int main (int argc, char** argv)
{
    /* DECLARATION OF CONSTANTS */

    /* Error message for an unknown environment error. */
    const char* const err_msg_env = "Unknown environment error.";

    /* Error message for the illegal number of additional arguments. */
    const char* const err_msg_argc =
        "Number of additional arguments must be 4: number of polygons to read, "
            "number of vertices, input file path and output file path.";

    /* Error message for the illegal number of polygons to read. */
    const char* const err_msg_np =
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

    /* DECLARATION OF VARIABLES */

    /* Number of polygons to read. */
    size_t N;

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
    N = 0U;

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
    N = (size_t)atoi(*(argv + 1U));

    /* Scan the number of vertices. */
    n = (size_t)atoi(*(argv + 2U));

    /* If the number of polygons to read is 0, print the error message and exit
     * with a non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_np);

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
    P = (real_t*)malloc(((4U * n) << 1U) * sizeof *P);

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
    memset(P, 0, ((4U * n) << 1U) * sizeof *P);

    /* Open the input file. */
    in = fopen(*(argv + 3U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!in)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((4U * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Open the output file. */
    out = fopen(*(argv + 4U), file_out_open_mode);

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
        memset(P, 0, ((4U * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read and generate polygons. */
    for (i = 0U; i < N; ++i)
    {
        /* Clear the memory in the array of points. */
        memset(P, 0, ((4U * n) << 1U) * sizeof *P);

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
                memset(P, 0, ((4U * n) << 1U) * sizeof *P);

                /* Deallocate memory for the polygons. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

        /* Copy the coordinates of the polygon three times. */
        memcpy(P + (n << 1U), P, (n << 1U) * sizeof *P);
        memcpy(P + (n << 2U), P, (n << 1U) * sizeof *P);
        memcpy(P + 6U * n, P, (n << 1U) * sizeof *P);

        /* Generate rectangular rotations of the polygon. */
        rect_rotate_polygon(n, P + (n << 1U), 1);
        rect_rotate_polygon(n, P + (n << 2U), 2);
        rect_rotate_polygon(n, P + 6U * n, 3);

        /* Correct orientations of generated polygons. */
        correct_polygon_orientation(n, P + (n << 1U));
        correct_polygon_orientation(n, P + (n << 2U));
        correct_polygon_orientation(n, P + 6U * n);

        /* Dump the polygons to the output file. */
        dump_polygons(out, n, P, 4U);
    }

    /* Close the output file. */
    fclose(out);
    out = (FILE*)(NULL);

    /* Close the input file. */
    fclose(in);
    in = (FILE*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, ((4U * n) << 1U) * sizeof *P);

    /* Deallocate memory for the polygons. */
    free(P);
    P = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
