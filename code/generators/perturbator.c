/**
 * Program for generating polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./perturbate N0 n in N1 sd out
 * where:
 *     N0  is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     in  is the path to the input file to read the original coordinates of
 *         vertices,
 *     N1  is the number of polygons to generate from each read polygon (at
 *         least 1),
 *     sd  is the standard deviation of the normal distribution of displacement
 *         (strictly positive),
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
 * polygon generated from the i-th polygon is generated by displacing vertices
 * of the first polygon by choosing a uniformly distributed random angle of
 * displacement and normally distributed random distance of displacement with
 * the standard deviation of sd.  Only after generating all the N1 - 1 polygons
 * from the i-th polygon is the original polygon normalised to be of diameter 1
 * (up to a numerical precision).
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
        "Number of additional arguments must be 6: number of polygons to read, "
            "number of vertices, input file path, number of polygons to "
            "generate, standard deviation of displacement and output file "
            "path.";

    /* Error message for the illegal number of polygons to read. */
    const char* const err_msg_npr =
        "Number of polygons to read must be at least 1.";

    /* Error message for the illegal number of vertices. */
    const char* const err_msg_nv = "Number of vertices must be at least 3.";

    /* Error message for the illegal number of polygons to generate. */
    const char* const err_msg_npg =
        "Number of polygons to generate must be at least 1.";

    /* Error message for the illegal standard deviation. */
    const char* const err_msg_sd =
        "Standard deviation must be strictly positive.";

    /* Error message for the memory allocation fail. */
    const char* const err_msg_mem = "Memory allocation fail.";

    /* Error message for exceeding the maximal number of attempts. */
    const char* const err_msg_att = "Maximal number of attempts exceeded.";

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

    /* Expected number of vertices and true number of vertices. */
    size_t n;
    size_t n_true;

    /* Standard deviation of displacement. */
    real_t sd;

    /* Array of vertices. */
    real_t* P;

    /* Arrays of the differences in coordinates, the lengths of edges and the
     * outer angles. */
    real_t* dx;
    real_t* dy;
    real_t* l;
    real_t* phi;

    /* Input file. */
    FILE* in;

    /* Output file. */
    FILE* out;

    /* Iteration indices. */
    size_t i;
    size_t j;
    size_t k;

    /* INITIALISATION OF VARIABLES */

    /* Number of polygons to read. */
    N0 = 0U;

    /* Number of polygons to generate. */
    N1 = 0U;

    /* Expected number of vertices and true number of vertices. */
    n = 0U;
    n_true = 0U;

    /* Standard deviation of displacement. */
    sd = 0.0;

    /* Array of vertices. */
    P = (real_t*)(NULL);

    /* Arrays of the differences in coordinates, the lengths of edges and the
     * outer angles. */
    dx = (real_t*)(NULL);
    dy = (real_t*)(NULL);
    l = (real_t*)(NULL);
    phi = (real_t*)(NULL);

    /* Input file. */
    in = (FILE*)(NULL);

    /* Output file. */
    out = (FILE*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;
    k = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* If the number of additional command line arguments is not 6, print the
     * error message and exit with a non-zero value. */
    if (!(argc == 7))
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

    /* If any of the 7 command line arguments is a null-pointer, print the error
     * message and exit with a non-zero value. */
    if (
        !(
            *argv &&
            *(argv + 1U) &&
            *(argv + 2U) &&
            *(argv + 3U) &&
            *(argv + 4U) &&
            *(argv + 5U) &&
            *(argv + 6U)
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

    /* Scan the standard deviation of displacement. */
    sd = (real_t)atof(*(argv + 5U));

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

    /* If the standard deviation of displacement is not strictly positive, print
     * the error message and exit with a non-zero value. */
    if (!(0.0 < sd))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_sd);

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

    /* Allocate memory for the array of the differences in coordinates, the
     * lengths of edges and the outer angles. */
    dx = (real_t*)malloc(n * sizeof *dx);
    dy = (real_t*)malloc(n * sizeof *dy);
    l = (real_t*)malloc(n * sizeof *l);
    phi = (real_t*)malloc(n * sizeof *phi);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!(dx && dy && l && phi))
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_mem);

        /* Deallocate memory for the arrays of the differences in
         * coordinates, the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(dy);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the differences in coordinates, the lengths of edges and the
     * outer angles to zeros. */
    memset(dx, 0, n * sizeof *dx);
    memset(dy, 0, n * sizeof *dy);
    memset(l, 0, n * sizeof *l);
    memset(phi, 0, n * sizeof *phi);

    /* Save the number of vertices to the `saved_nn_integer` function. */
    saved_nn_integer(n);

    /* Save the standard deviation to the `saved_number` function. */
    saved_number(sd);

    /* Save the `constant_length` function to the `saved_len_generator`
     * function. */
    saved_len_generator(constant_length);

    /* Open the input file. */
    in = fopen(*(argv + 3U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!in)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

        /* Deallocate memory for the arrays of the differences in
         * coordinates, the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(dy);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

        /* Clear the memory in the array of points. */
        memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

        /* Deallocate memory for the polygons. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Open the output file. */
    out = fopen(*(argv + 6U), file_out_open_mode);

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

        /* Clear the memory in the arrays of the differences in coordinates,
         * the lengths of edges and the outer angles. */
        memset(dx, 0, n * sizeof *dx);
        memset(dy, 0, n * sizeof *dy);
        memset(l, 0, n * sizeof *l);
        memset(phi, 0, n * sizeof *phi);

        /* Deallocate memory for the arrays of the differences in
         * coordinates, the lengths of edges and the outer angles. */
        free(dx);
        dx = (real_t*)(NULL);
        free(dy);
        dy = (real_t*)(NULL);
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

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

                /* Clear the memory in the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                memset(dx, 0, n * sizeof *dx);
                memset(dy, 0, n * sizeof *dy);
                memset(l, 0, n * sizeof *l);
                memset(phi, 0, n * sizeof *phi);

                /* Deallocate memory for the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                free(dx);
                dx = (real_t*)(NULL);
                free(dy);
                dy = (real_t*)(NULL);
                free(l);
                l = (real_t*)(NULL);
                free(phi);
                phi = (real_t*)(NULL);

                /* Clear the memory in the array of points. */
                memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

                /* Deallocate memory for the polygons. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

        /* Correct the first polygon's orientation and enumeration. */
        correct_polygon_orientation(n, P);

        /* Describe the first polygon. */
        describe_polygon(n, P, dx, dy, l, phi);

        /* Save the polygon and its information to the `saved_polygon`,
         * `saved_lengths` and `saved_angles` functions. */
        saved_polygon(P);
        saved_lengths(l);
        saved_angles(phi);

        /* Generate the rest `N1` - 1 polygons. */
        for (j = 1U; j < N1; ++j)
        {
            /* Try to generate the `j`-th polygon. */
            for (k = 0U; k < (ATTEMPT_MAX); ++k)
            {
                /* Set the value of number `n_true` to the value of `n`. */
                n_true = n;

                /* Generate an array of `n` points using the
                 * `smart_random_polygon` function. */
                smart_random_polygon(
                    n,
                    P + ((j * n) << 1U),
                    perturbate_coordinate,
                    OUT_ITER_MAX,
                    IN_ITER_MAX
                );

                /* Reorder the vertices by flipping the array from index 2 to
                 * the end. */
                flip(P + ((j * n) << 1U) + 2U, n - 1U, 2U * sizeof *P);

                /* Simplify and check the array for the polygon.  If the array
                 * represents a true `n`-gon, break the `for`-loop. */
                if (simplify_check_polygon(&n_true, P + ((j * n) << 1U)))
                    if (n_true == n)
                        break;

                /* Clear the memory in the array of points. */
                memset(P + ((j * n) << 1U), 0, (n << 1U) * sizeof *P);
            }

            /* If the maximal number of attempts was exceeded, print the error
             * message, close the input and the output files, deallocate memory
             * and exit with a non-zero value. */
            if (k >= (ATTEMPT_MAX))
            {
                /* Print the error message. */
                fprintf(stderr, format_err_msg, err_msg_att);

                /* Close the output file. */
                fclose(out);
                out = (FILE*)(NULL);

                /* Close the input file. */
                fclose(in);
                in = (FILE*)(NULL);

                /* Clear the memory in the arrays of the differences
                 * in coordinates, the lengths of edges and the outer angles. */
                memset(dx, 0, n * sizeof *dx);
                memset(dy, 0, n * sizeof *dy);
                memset(l, 0, n * sizeof *l);
                memset(phi, 0, n * sizeof *phi);

                /* Deallocate memory for the arrays of the differences in
                 * coordinates, the lengths of edges and the outer angles. */
                free(dx);
                dx = (real_t*)(NULL);
                free(dy);
                dy = (real_t*)(NULL);
                free(l);
                l = (real_t*)(NULL);
                free(phi);
                phi = (real_t*)(NULL);

                /* Clear the memory in the array of points. */
                memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

                /* Deallocate memory for the polygons. */
                free(P);
                P = (real_t*)(NULL);

                /* Exit with a non-zero value. */
                exit(EXIT_FAILURE);
            }

            /* Normalise the `j`-th polygon. */
            normalise_polygon(n, P + ((j * n) << 1U));
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

    /* Clear the memory in the arrays of the differences in coordinates,
     * the lengths of edges and the outer angles. */
    memset(dx, 0, n * sizeof *dx);
    memset(dy, 0, n * sizeof *dy);
    memset(l, 0, n * sizeof *l);
    memset(phi, 0, n * sizeof *phi);

    /* Deallocate memory for the arrays of the differences in
     * coordinates, the lengths of edges and the outer angles. */
    free(dx);
    dx = (real_t*)(NULL);
    free(dy);
    dy = (real_t*)(NULL);
    free(l);
    l = (real_t*)(NULL);
    free(phi);
    phi = (real_t*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, ((N1 * n) << 1U) * sizeof *P);

    /* Deallocate memory for the polygons. */
    free(P);
    P = (real_t*)(NULL);

    /* Return a zero value (exit with a zero value). */
    return EXIT_SUCCESS;
}
