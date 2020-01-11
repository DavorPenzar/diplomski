/**
 * Program for generating triangles.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./generate m out
 * where:
 *     m   is the number of dicretisation points on the x-axis (at least 2),
 *     out is the path to the output file to print the coordinates of vertices.
 *
 * The program generates triangles with vertices (1 / 2, 0), V_2, (-1 / 2, 0)
 * such that V_2 is not on the x-axis, is in the first quadrant or on the y-axis
 * and that |V_2, (1 / 2, 0)| <= |V_2, (-1 / 2, 0)| <= 1, where |P, Q| is the
 * distance from the point P to the point Q.
 *
 * Rectangle [0, 1 / 2] x [0, sqrt(3) / 2] is discretised with m points on the
 * x-axis and n points on the y-axis, where n / m is the best rational
 * approximation of sqrt(3) with positive denominator less than or equal to m.
 * Each of the point is tried for the triangle and, if it satisfies the
 * conditions, is printed to the output file.  However, some points are skipped
 * (the ones too close to the x-axis---minimal y-coordinate allowed is hard
 * coded).
 *
 * Each triangle is printed to the output file in its own line.  Each triangle
 * is formatted as
 *     x_0	y_0	x_1	y_1	x_2	y_2
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex ((1 / 2, 0) being the first vertex).  The
 * triangles are sorted lexicographically according to coordinates of the second
 * vertex.
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

    /* Minimal y-coordinate allowed. */
    const real_t y_min = 0.05;

    /* Numerical approximation of sqrt(3). */
    const real_t sqrt_3 =
        1.7320508075688772935274463415058723669428052538103806280558069795;

    /* Numerical approximation of sqrt(3) / 2. */
    const real_t half_sqrt_3 =
        0.86602540378443864676372317075293618347140262690519031402790348973;

    /* Error message for an unknown environment error. */
    const char* const err_msg_env = "Unknown environment error.";

    /* Error message for the illegal number of additional arguments. */
    const char* const err_msg_argc =
        "Number of additional arguments must be 2: number of dicretisation "
            "points on x-axis and path to the output file.";

    /* Error message for the illegal number of discretisation points. */
    const char* const err_msg_nd =
        "Number of discretisation points must be at least 2.";

    /* Error message for the illegal number of vertices. */
    const char* const err_msg_nD =
        "Number of discretisation points is too large.";

    /* Error message for output file opening fail. */
    const char* const err_msg_out = "Output file cannot be opened.";

    /* Format string for printing error messages. */
    const char* const format_err_msg = "%s\n";

    /* Mode of the output file to open. */
    const char* const file_open_mode = "wt";

    /* Format string for printing the number of generated triangles. */
    const char* const format_number_triangles = "Number of triangles: %lu.\n";

    /* DECLARATION OF VARIABLES */

    /* Number of triangles. */
    size_t N;

    /* Numbers of discretisation points on x-axis and y-axis. */
    size_t m;
    size_t n;

    /* Array of vertices. */
    real_t T[6U];

    /* Output file. */
    FILE* out;

    /* Auxiliary decremented numbers of discretisation points on x-axis and
     * y-axis (of type `real_t`). */
    real_t real_m_;
    real_t real_n_;

    /* Difference in x-coordinates. */
    real_t dx;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Number of triangles. */
    N = 0U;

    /* Numbers of discretisation points on x-axis and y-axis. */
    m = 0U;
    n = 0U;

    /* Output file. */
    out = (FILE*)(NULL);

    /* Auxiliary decremented numbers of discretisation points on x-axis and
     * y-axis (of type `real_t`). */
    real_m_ = 0.0;
    real_n_ = 0.0;

    /* Difference in x-coordinates. */
    dx = 0.0;

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* Initialise coordinates of vertices to zeros. */
    memset(T, 0, 6U * sizeof *T);

    /* If the number of additional command line arguments is not 3, print the
     * error message and exit with a non-zero value. */
    if (!(argc == 3))
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
    if (!(*argv && *(argv + 1U) && *(argv + 2U)))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_env);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the number of discretisation points on x-axis and compute the number
     * of discretisation points on y-axis. */
    m = (size_t)atoi(*(argv + 1U));
    n = (size_t)(sqrt_3 * (double)m + 0.5);

    /* If the number of discretisation points on x-axis is 0, print the error
     * message and exit with a non-zero value. */
    if (m < 2)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_nd);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the number of discretisation points was too large, print the error
     * message and exit with a non-zero value. */
    if (!(m < n))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_nD);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);

    }

    /* Extract the auxiliary decremented numbers of discretisation points. */
    real_m_ = (double)(m - 1U);
    real_n_ = (double)(n - 1U);

    /* Open the output file. */
    out = fopen(*(argv + 2U), file_open_mode);

    /* If the output file could not be opened, print the error message and exit
     * with a non-zero value. */
    if (!out)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_out);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Set the first vertex to (1 / 2, 0), and the third vertex to
     * (-1 / 2, 0). */
    *T = 0.5;
    *(T + 4U) = -0.5;

    /* Generate triangles. */
    for (i = 0U; i < m; ++i)
    {
        /* Set the x-coordinate of the next few triangles. */
        *(T + 2U) = 0.5 * (real_t)i / real_m_;

        /* Compute the square difference in x-coordinates of the second and the
         * third vertex. */
        dx = *(T + 2U) + 0.5;
        dx *= dx;

        /* Generate y-coordinates and complete triangles. */
        for (j = 1U; j < n; ++j)
        {
            /* Compute the y-coordinate of the current triangle. */
            *(T + 3U) = half_sqrt_3 * (real_t)j / real_n_;

            /* If y-coordinate is too low, continue to the next, higher
             * vertex. */
            if (rabs(*(T + 3U)) < y_min)
                continue;

            /* If the length of the edge from the second to the third vertex of
             * the current triangle exceeds 1, break the inner `for`-loop (all
             * higher vertices would also exceed this limit). */
            if (1.0 < dx + *(T + 3U) * *(T + 3U))
                break;

            /* Print the current triangle to the output file. */
            dump_polygons(out, 3U, T, 1U);

            /* Increment the number of generated triangles. */
            ++N;
        }
    }

    /* Close the output file. */
    fclose(out);
    out = (FILE*)(NULL);

    /* Print the number of generated triangles. */
    printf(format_number_triangles, N);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
