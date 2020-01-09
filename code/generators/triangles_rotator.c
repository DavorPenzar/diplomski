/**
 * Program for generating triangles.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./perturbate N in out
 * where:
 *     N   is the number of triangles (at least 1),
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
 *     out is the path to the output file to print the coordinates of vertices.
 *
 * Each triangle must be formated in the input file as
 *     x_0 y_0 x_1 y_1 x_2 y_2
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.  Whitespaces may differ (they may even be
 * spaces, tabs, line breaks...).  It is believed that each input triangle
 * truly represents a triangle---this is not checked and if any input triangle
 * does not satisfy this, results may be unexpected.
 *
 * Note that the input file must contain at least N triangles.  If, however, it
 * contains more than N triangles, only the first N0 polygons are read and used
 * to generate new triangles.
 *
 * Suppose the i-th triangle is read from the input file (0 <= i < N0).  The
 * first polygon generated from the i-th triangle is the i-th triangle itself
 * (not even transposed, rotated, reflexed and/or scaled).  Two consecutive
 * triangles generated are its rotations so that its each edge becomes parallel
 * to the original triangle's edge ending in its lowest vertex (and the most to
 * the right if there is a tie).  Only after generating all three triangles from
 * from the i-th triangle is the original triangle normalised to be of diameter
 * 1 (up to a numerical precision).
 *
 * Each triangle is printed to the output file in its own line.  Each triangle
 * is formatted as
 *     x_0	y_0	x_1	y_1	x_2_1	y_2
 * where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
 * y-coordinate of the i-th vertex.  It is guaranteed that the triangles are of
 * diameter 1 (up to a numerical precision) and that they fit in the
 * [-1 / 2, 1 / 2] x [-1 / 2, 1 / 2] square in the plane.  The triangles are
 * printed in the order of reading and generating.
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
        "Number of additional arguments must be 3: number of triangles, input "
            "input file path and output file path.";

    /* Error message for the illegal number of triangles. */
    const char* const err_msg_nt =
        "Number of polygons to read must be at least 1.";

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

    /* Number of triangles. */
    size_t N;

    /* Array of vertices. */
    real_t P[18U];

    /* Arrays of the differences in coordinates, the lengths of edges and the
     * outer angles. */
    real_t dx[3U];
    real_t dy[3U];
    real_t l[3U];
    real_t phi[3U];

    /* Input file. */
    FILE* in;

    /* Output file. */
    FILE* out;

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Number of triangles. */
    N = 0U;

    /* Input file. */
    in = (FILE*)(NULL);

    /* Output file. */
    out = (FILE*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* Initialise coordinates of vertices of polygons to zeros. */
    memset(P, 0, 18U * sizeof *P);

    /* Initialise the differences in coordinates, the lengths of edges and the
     * outer angles to zeros. */
    memset(dx, 0, 3U * sizeof *dx);
    memset(dy, 0, 3U * sizeof *dy);
    memset(l, 0, 3U * sizeof *l);
    memset(phi, 0, 3U * sizeof *phi);

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

    /* Scan the number of triangles. */
    N = (size_t)atoi(*(argv + 1U));

    /* If the number of triangles is 0, print the error message and exit with a
     * non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_nt);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Open the input file. */
    in = fopen(*(argv + 2U), file_in_open_mode);

    /* If the input file could not be opened, print the error message, clear
     * memory and exit with a non-zero value. */
    if (!in)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, 3U * sizeof *dx);
        memset(dy, 0, 3U * sizeof *dy);
        memset(l, 0, 3U * sizeof *l);
        memset(phi, 0, 3U * sizeof *phi);

        /* Clear the memory in the array of points. */
        memset(P, 0, 18U * sizeof *P);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Open the output file. */
    out = fopen(*(argv + 3U), file_out_open_mode);

    /* If the output file could not be opened, print the error message,
     * close the input file, clear memory and exit with a non-zero value. */
    if (!out)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_out);

        /* Close the input file. */
        fclose(in);
        in = (FILE*)(NULL);

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, 3U * sizeof *dx);
        memset(dy, 0, 3U * sizeof *dy);
        memset(l, 0, 3U * sizeof *l);
        memset(phi, 0, 3U * sizeof *phi);

        /* Clear the memory in the array of points. */
        memset(P, 0, 18U * sizeof *P);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read and rotate triangles. */
    for (i = 0U; i < N; ++i)
    {
        /* Clear the memory in the array of points. */
        memset(P, 0, 18U * sizeof *P);

        /* Read the coordinates of the `i`-th input triangle.  If any of the
         * coordinates could not be read, print the error message, close the
         * output and the input files, clear memory and exit with a non-zero
         * value. */
        for (j = 0U; (j >> 1U) < 3U; ++j)
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

                /* Clear the memory in the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                memset(dx, 0, 3U * sizeof *dx);
                memset(dy, 0, 3U * sizeof *dy);
                memset(l, 0, 3U * sizeof *l);
                memset(phi, 0, 3U * sizeof *phi);

                /* Clear the memory in the array of points. */
                memset(P, 0, 18U * sizeof *P);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

        /* Correct the triangles's orientation and enumeration. */
        correct_polygon_orientation(3U, P);

        /* Describe the triangle. */
        describe_polygon(3U, P, dx, dy, l, phi);

        /* Copy the coordinates of the triangle two times. */
        memcpy(P + 6U, P, 6U * sizeof *P);
        memcpy(P + 12U, P, 6U * sizeof *P);

        /* Generate rotations of the triangle. */
        rotate_polygon(3U, P + 6U, *(phi + 1U));
        rotate_polygon(3U, P + 12U, *phi + *(phi + 1U));

        /* Normalise triangles. */
        normalise_polygon(3U, P);
        normalise_polygon(3U, P + 6U);
        normalise_polygon(3U, P + 12U);

        /* Describe the first triangle. */
        describe_polygon(3U, P, dx, dy, l, phi);

        /* Dump the triangles to the output file. */
        dump_polygons(out, 3U, P, 3U);
    }

    /* Close the output file. */
    fclose(out);
    out = (FILE*)(NULL);

    /* Close the input file. */
    fclose(in);
    in = (FILE*)(NULL);

    /* Clear the memory in the arrays of the differences in coordinates,
     * the lengths of edges and the outer angles. */
    memset(dx, 0, 3U * sizeof *dx);
    memset(dy, 0, 3U * sizeof *dy);
    memset(l, 0, 3U * sizeof *l);
    memset(phi, 0, 3U * sizeof *phi);

    /* Clear the memory in the array of points. */
    memset(P, 0, 18U * sizeof *P);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
