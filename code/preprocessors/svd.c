/**
 * Program for describing polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * Usage:
 *     ./svd in N n out
 * where:
 *     in  is the path to the input file to read the lengths of edges and the
 *         outer angles,
 *     N   is the number of polygons to read (at least 1),
 *     n   is the number of vertices of each polygon (at least 3),
 *     out is the path to the output file to print the singular values of the
 *         lengths of edges and the outer angles.
 *
 * Each polygon must be formated in the input file as
 *     l_0	l_1	...	l_n_minus_one	phi_1	phi_2	...	phi_n_minus_1	phi_0
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
 * Each polygon's singular value are printed in the input file as
 *     s_l_0	s_l_1	...	s_l_n_minus_one	s_phi_0	s_phi_1	...	s_phi_n_minus_1
 * where s_l_i the i-th largest (counting their multiplicities) singular value
 * of the lengths of edges and s_phi_i is the i-th largest (counting their
 * multiplicities) singular value of the outer angles of the polygon.  If the
 * singular values could not be computed (illegal values or the algorithm did
 * not converge), the singular values are set to zeros and the program continues
 * on.
 *
 * The pogram prints to the console the time elapsed only during the computation
 * of the singular values.  Time needed to read and print is not measured.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Compile with mathematical constants from "math.h". */
#define _USE_MATH_DEFINES   1

/* Compile using the DGESVD driver. */
#define _USE_SVD_DRIVER 1

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

    /* Numerical approximation of the mathematical constant pi. */
    const real_t pi =
        3.1415926535897932384626433832795028841971693993751058209749445923;

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

    /* Arrays of the singular values of the lengths of edges and the outer
     * angles. */
    real_t* s_l;
    real_t* s_phi;

    /* Auxiliary array for matrices of unoriented representations of array of
     * the lengths of edges and the outer angles. */
    real_t* A;

    /* Leading dimension of the auxiliary matrix `A`. */
    size_t ld_A;

    /* Information from the SVD drivers. */
    int info;

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

    /* Arrays of the singular values of the lengths of edges and the outer
     * angles. */
    s_l = (real_t*)(NULL);
    s_phi = (real_t*)(NULL);

    /* Auxiliary array for matrices of unoriented representations of array of
     * the lengths of edges and the outer angles. */
    A = (real_t*)(NULL);

    /* Leading dimension of the auxiliary matrix `A`. */
    ld_A = 0U;

    /* Information from the SVD drivers. */
    info = 0;

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

    /* Allocate memory for the arrays of the singular values of the lengths of
     * edges and the outer angles. */
    s_l = (real_t*)malloc(((N * n) << 1U) * sizeof *s_l);

    /* If the memory allocation has failed, deallocate memory, print the error
     * message and exit with a non-zero value. */
    if (!l)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_mem);

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

    /* Initialise the singular values of the lengths of edges and the outer
     * angles to zeros. */
    memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);
    s_phi = s_l + n;

    /* Compute the leading dimension of the auxiliary matrix `A`. */
    ld_A = ((n + 63U) >> 6U) << 6U;

    /* Allocate memory for the auxiliary matrix. */
    A = (real_t*)malloc(((n * ld_A) << 1U) * sizeof *A);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!A)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the arrays of the singular values of the lengths
         * of edges and the outer angles. */
        memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);

        /* Deallocate memory for the arrays of the singular values of the
         * lengths of edges and the outer angles. */
        free(s_l);
        s_l = (real_t*)(NULL);
        s_phi = (real_t*)(NULL);

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

    /* Initialise the auxiliary matrix to zeros. */
    memset(A, 0, ((n * ld_A) << 1U) * sizeof *A);

    /* Open the input file. */
    inout = fopen(*(argv + 1U), file_in_open_mode);

    /* If the input file could not be opened, print the error message,
     * deallocate memory and exit with a non-zero value. */
    if (!inout)
    {
        /* Print the error message. */
        fprintf(stderr, format_err_msg, err_msg_in);

        /* Clear the memory in the auxiliary matrix. */
        memset(A, 0, ((n * ld_A) << 1U) * sizeof *A);

        /* Deallocate memory for the auxiliary matrix. */
        free(A);
        A = (real_t*)(NULL);

        /* Clear the memory in the arrays of the singular values of the lengths
         * of edges and the outer angles. */
        memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);

        /* Deallocate memory for the arrays of the singular values of the
         * lengths of edges and the outer angles. */
        free(s_l);
        s_l = (real_t*)(NULL);
        s_phi = (real_t*)(NULL);

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

                /* Clear the memory in the auxiliary matrix. */
                memset(A, 0, ((n * ld_A) << 1U) * sizeof *A);

                /* Deallocate memory for the auxiliary matrix. */
                free(A);
                A = (real_t*)(NULL);

                /* Clear the memory in the arrays of the singular values of the
                 * lengths of edges and the outer angles. */
                memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);

                /* Deallocate memory for the arrays of the singular values of
                 * the lengths of edges and the outer angles. */
                free(s_l);
                s_l = (real_t*)(NULL);
                s_phi = (real_t*)(NULL);

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
        /* Divide all the outer angles by `pi`. */
        for (j = 0U; j < n; ++j)
            *(phi + ((i * n) << 1U) + j) /= pi;

        /* Compute the singular values of the lengths of edges of the `i`-th
         * polygon. */
        svd_polygon(n, l + ((i * n) << 1U), s_l + ((i * n) << 1U), A, &info);

        /* If the computation has failed, clear the singular values of the
         * lengths of edges of the `i`-th polygon. */
        if (info)
            memset(s_l + ((i * n) << 1U), 0, n * sizeof *s_l);

        /* Compute the singular values of the outer angles of the `i`-th
         * polygon. */
        svd_polygon(
            n,
            phi + ((i * n) << 1U),
            s_phi + ((i * n) << 1U),
            A,
            &info
        );

        /* If the computation has failed, clear the singular values of the
         * outer angles of the `i`-th polygon. */
        if (info)
            memset(s_phi + ((i * n) << 1U), 0, n * sizeof *s_phi);
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

        /* Clear the memory in the auxiliary matrix. */
        memset(A, 0, ((n * ld_A) << 1U) * sizeof *A);

        /* Deallocate memory for the auxiliary matrix. */
        free(A);
        A = (real_t*)(NULL);

        /* Clear the memory in the arrays of the singular values of the lengths
         * of edges and the outer angles. */
        memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);

        /* Deallocate memory for the arrays of the singular values of the
         * lengths of edges and the outer angles. */
        free(s_l);
        s_l = (real_t*)(NULL);
        s_phi = (real_t*)(NULL);

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

    /* Dump the singular values to the output file.  Note that all polygons have
     * exactly 2 * `n` singular values stored in the array `s_l`, so the
     * `dump_polygons` function can be used to dump the singular values. */
    dump_polygons(inout, n, l, N);

    /* Close the output file. */
    fclose(inout);
    inout = (FILE*)(NULL);

    /* Clear the memory in the auxiliary matrix. */
    memset(A, 0, ((n * ld_A) << 1U) * sizeof *A);

    /* Deallocate memory for the auxiliary matrix. */
    free(A);
    A = (real_t*)(NULL);

    /* Clear the memory in the arrays of the singular values of the lengths of
     * edges and the outer angles. */
    memset(s_l, 0, ((N * n) << 1U) * sizeof *s_l);

    /* Deallocate memory for the arrays of the singular values of the lengths of
     * edges and the outer angles. */
    free(s_l);
    s_l = (real_t*)(NULL);
    s_phi = (real_t*)(NULL);

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
