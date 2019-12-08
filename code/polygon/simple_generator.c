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

/* Define constants for the numerical approximation of the mathematical constant
 * pi and some of its fractions. */
#define PI      3.1415926535897932384626433832795028841971693993751058209749445923L
#define PI_2    1.5707963267948966192313216916397514420985846996875529104874722962L
#define PI_3    1.0471975511965977461542144610931676280657231331250352736583148641L
#define PI_4    0.7853981633974483096156608458198757210492923498437764552437361481L
#define PI_5    0.7853981633974483096156608458198757210492923498437764552437361481L
#define PI_6    0.5235987755982988730771072305465838140328615665625176368291574321L
#define PI_7    0.4487989505128276054946633404685004120281670570535865458535635132L
#define PI_8    0.3926990816987241548078304229099378605246461749218882276218680740L
#define PI_9    0.3490658503988659153847381536977225426885743777083450912194382880L
#define PI_10   0.3141592653589793238462643383279502884197169399375105820974944592L
#define PI_11   0.2855993321445266580420584893890457167451972181250096200886313266L
#define PI_12   0.2617993877991494365385536152732919070164307832812588184145787160L

/* Define constants for converting radians to degrees and vice versa. */
#define DEG_TO_RAD  57.295779513082320876798154814105170332405472466564321549160243861L
#define RAD_TO_DEG   0.017453292519943295769236907684886127134428718885417254560971914L

/* Define constants for maximal numbers of iterations. */
#define IN_ITER_MAX     1024U
#define OUT_ITER_MAX    1024U
#define ATTEMPT_MAX     1024U

/* Define functions. */

/**
 * Save a non-negative integer or get the saved non-negative integer.
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param n
 *     Non-negative integer to save.  If `n` == `0`, the number `n` is not
 *     saved.
 *
 * @return
 *     Last non-negative integer that was saved (if `n` != `0`, the value of `n`
 *     is returned).  Initially 0 (before saving any non-negative integer).
 *
 */
size_t saved_nn_integer (size_t n)
{
    /* Saved non-negative integer. */
    static size_t saved_n = 0U;

    /* If the argument `n` is not 0, set `saved_n` to `n` (save the number). */
    if (n)
        saved_n = n;

    /* Return the saved non-negative integer. */
    return saved_n;
}

/**
 * Save a real number or get the saved real number.
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param x
 *     Real number to save.  If `rabs(x)` == `0`, the number `x` is not saved.
 *
 * @return
 *     Last real number that was saved (if `rabs(x)` != `0`, the value of `x` is
 *     returned).  Initially 0 (before saving any real number).
 *
 * @see rabs
 *
 */
real_t saved_number (real_t x)
{
    /* Saved real number. */
    static real_t saved_x = 0.0;

    /* If the argument `x` is not 0, set `saved_x` to `x` (save the number). */
    if (!(rabs(x) == 0.0))
        saved_x = x;

    /* Return the saved real number. */
    return saved_x;
}

/**
 * Save an array of real numbers (preferably an array of coordinates of vertices
 * of a polygon).
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param P
 *     Array of real numbers to save.  If `P` is a null pointer, the "array" `P`
 *     is not saved.
 *
 * @return
 *     Last array of real number that was saved (if `P` != `NULL`, the value of
 *     `P` is returned).  Initially `NULL` (before saving any array of real
 *     numbers).
 *
 */
const real_t* saved_polygon (const real_t* P)
{
    /* Saved polygon. */
    static const real_t* saved_P = (real_t*)(NULL);

    /* If the argument `P` is not a null-pointer, set `saved_P` to `P` (save the
     * polygon). */
    if (P)
        saved_P = P;

    /* Return the saved polygon. */
    return saved_P;
}

/**
 * Save an array of real numbers (preferably an array of lengths of edges of a
 * polygon).
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param l
 *     Array of real numbers to save.  If `l` is a null pointer, the "array" `l`
 *     is not saved.
 *
 * @return
 *     Last array of real number that was saved (if `l` != `NULL`, the value of
 *     `l` is returned).  Initially `NULL` (before saving any array of real
 *     numbers).
 *
 */
const real_t* saved_lengths (const real_t* l)
{
    /* Saved lengths. */
    static const real_t* saved_l = (real_t*)(NULL);

    /* If the argument `l` is not a null-pointer, set `saved_l` to `l` (save the
     * lengths). */
    if (l)
        saved_l = l;

    /* Return the saved lengths. */
    return saved_l;
}

/**
 * Save an array of real numbers (preferably an array of outer angles of a
 * polygon).
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param phi
 *     Array of real numbers to save.  If `phi` is a null pointer, the "array"
 *     `phi` is not saved.
 *
 * @return
 *     Last array of real number that was saved (if `phi` != `NULL`, the value
 *     of `phi` is returned).  Initially `NULL` (before saving any array of real
 *     numbers).
 *
 */
const real_t* saved_angles (const real_t* phi)
{
    /* Saved outer angles. */
    static const real_t* saved_phi = (real_t*)(NULL);

    /* If the argument `phi` is not a null-pointer, set `saved_phi` to `phi`
     * (save the outer angles). */
    if (phi)
        saved_phi = phi;

    /* Return the saved outer angles. */
    return saved_phi;
}

/**
 * Save a function for combining two real numbers.
 *
 * A function for combinig two real numbers may be, for eample, `rmin`, `rmax`,
 * a function that computes the average of the numbers, a constant...
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param combiner
 *     Function for combinig two real numbers.  If `combiner == NULL`, the
 *     "function" `combiner` is not saved.
 *
 * @return
 *     Last function for combining two real numbers that was saved (if
 *     `combiner` != `NULL`, the value of `combiner` is returned).  Initially
 *     `NULL` (before saving any function for combining two real numbers).
 *
 * @see rmin
 * @see rmax
 *
 */
real_t (
    * saved_comb_function (real_t (* combiner) (real_t, real_t))
) (real_t, real_t)
{
    /* Saved combining function. */
    static real_t (* saved_combiner) (real_t, real_t) =
        (real_t (*) (real_t, real_t))(NULL);

    /* If the argument `combiner` is not a null-pointer, set `saved_combiner` to
     * `combiner` (save the combining function). */
    if (combiner)
        saved_combiner = combiner;

    /* Return the saved combining function. */
    return saved_combiner;
}

/**
 * Save a function for generating lengths (real values).
 *
 * A function for generating lengths may be, for eample, `constant_length` or
 * `combiner_length`.
 *
 * Caution: the function uses static variables to save the values. If more than
 * one thread needs to call the function and it is likely that they will do it
 * in the same time, use a blocking mechanism such as mutex to prevent
 * interference.
 *
 * @param generator
 *     Function for generating lengths.  If `generator == NULL`, the "function"
 *     `generator` is not saved.
 *
 * @return
 *     Last function for generating lengths that was saved (if
 *     `generator` != `NULL`, the value of `generator` is returned).  Initially
 *     `NULL` (before saving any function for generating lengths).
 *
 * @see constant_length
 * @see combiner_length
 *
 */
real_t (* saved_len_generator (real_t (* generator) (size_t))) (size_t)
{
    /* Saved length generator (function). */
    static real_t (* saved_generator) (size_t) = (real_t (*) (size_t))(NULL);

    /* If the argument `generator` is not a null-pointer, set `saved_generator`
     * to `generator` (save the length generator). */
    if (generator)
        saved_generator = generator;

    /* Return the saved generator. */
    return saved_generator;
}

/**
 * Generate a constant length.
 *
 * The function is actually only a wrapper around the `saved_number` function
 * (the returned value is generated by calling `saved_number(0.0)`).  The
 * purpose of the function is to save it to the `saved_len_generator` function.
 *
 * The function is intended to be saved to the `saved_comb_function` function.
 *
 * @param i
 *     Dummy argument.  Required for compatibility with the function
 *     `saved_len_generator`.
 *
 * @return
 *     A constant length (actually the number saved to the `saved_number`
 *     function).
 *
 * @see saved_number
 * @see saved_len_generator
 *
 */
real_t constant_length (size_t i)
{
    return saved_number(0.0);
}

/**
 * Generate a length by combinig lengths of edges of the saved polygon.
 *
 * The function generates the value by calling the `saved_comb_function`
 * function.
 *
 * The function is intended to be saved to the `saved_comb_function`.
 *
 * @param i
 *     Index of the vertex of the saved polygon.
 *
 * @return
 *     The value generated by calling the function saved to the
 *     `saved_comb_function` function by combining the (`(i` - 1) mod `n`)-th
 *     and the `i`-th value in the array saved to the `saved_lengths` function,
 *     where `n` is the non-negative integer saved to the `saved_nn_integer`
 *     function.
 *
 * @see saved_nn_integer
 * @see saved_lengths
 * @see saved_comb_function
 * @see saved_len_generator
 *
 */
real_t combiner_length (size_t i)
{
    /* DECLARATION OF VARIABLES */

    /* Number of edges. */
    size_t n;

    /* Lengths of edges. */
    const real_t* l;

    /* Function for combining two real values. */
    real_t (* combiner) (real_t, real_t);

    /* Value to compute and return. */
    real_t return_value;

    /* INITIALISATION OF VARIABLES */

    /* Number of edges. */
    n = 0U;

    /* Lengths of edges. */
    l = (real_t*)(NULL);

    /* Function for combining two real values. */
    combiner = (real_t (*) (real_t, real_t))(NULL);

    /* Value to compute and return. */
    return_value = 0.0;

    /* ALGORITHM */

    /* Fetch the saved non-negative integer. */
    n = saved_nn_integer(0U);

    /* Fetch the saved lengths of edges. */
    l = saved_lengths((real_t*)(NULL));

    /* Fetch the saved combining function. */
    combiner = saved_comb_function((real_t (*) (real_t, real_t))(NULL));

    /* If the index `i` is strictly less than the number of edges `n`, compute
     * the `return_value` by combining the edge at index (`i` - 1) mod `n` and
     * at index `i`. */
    if (i < n)
        return_value = (*combiner)(*(l + decmod(i, n)), *(l + i));

    /* Return the return value. */
    return return_value;
}

/**
 * Scan the coordinate from the console.
 *
 * The coordinate is queried by printing using the `printf` function and scanned
 * using the `scanf` function.
 *
 * The purpose of the function is to pass it as the argument to the
 * `random_polygon` and the `smart smart_random_polygon` functions.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     Value scanned from the console.
 *
 * @see printf
 * @see scanf
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
real_t scan_coordinate (size_t i, size_t coordinate)
{
    /* DECLARATION OF VARIABLES */

    /* Value to scan and return. */
    real_t return_value;

    /* INITIALISATION OF VARIABLES */

    /* Value to svcan and return. */
    return_value = 0.0;

    /* ALGORITHM */

    /* Increment the index `i`. */
    ++i;

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Print the query to console. */
    printf(
        "Enter %c-coordinate of the %lu%s point: ",
            (coordinate == 0U) ? 'x' : ((coordinate == 1U) ? 'y' : '?'),
            i,
            (i % 100U == 11U || i % 100U == 12U || i % 100U == 13U) ?
                "th" :
                (
                    (i % 10U == 1U) ?
                        "st" :
                        (
                            (i % 10U == 2U) ?
                                "nd" :
                                ((i % 10U == 3U) ? "rd" : "th")
                        )
                )
    );

    /* Scan the value from the console. */
    do
    {
        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);
    }
    while (!(scanf(" %lf", &return_value) == 1));

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Return the scanned value. */
    return return_value;
}

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
 * @see rrand
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
real_t random_coordinate (size_t i, size_t coordinate)
{
    return rrand();
}

/**
 * Generate a coordinate by perturbating the saved coordinate.
 *
 * The coordinates are being generated by using the values saved to the
 * `saved_polygon` function and perturbating them by generating a uniformly
 * distributed angle from the interval [0, pi] (using the `rrand` function) and
 * by generating a normally distributed length (using the `rrandn` function) and
 * then translating the original point in the generated direction so that the
 * distance from the original point and the generated point equals the absolute
 * value of the generated length.  If the generated length is negative, the
 * effect is the same as if the generated angle was incremented by pi and the
 * length was positive (absolute value of the negative length).  The standard
 * deviation of the generated length is computed by taking the absolute value
 * (using the `rabs` function) of the value returned by the function saved to
 * the `saved_len_generator` function by passing the value of the parameter `i`
 * as the argument.  If the length generated by the function saved to the
 * `saved_len_generator` is 0, the new point will be coordinately the same as
 * the original point.  The angle and the length are generated only when the
 * value of the parameter `coordinate` is 0 (when the x-coordinate is to be
 * generated).
 *
 * The purpose of the function is to pass it as the argument to the
 * `random_polygon` and the `smart smart_random_polygon` functions.
 *
 * Caution: the function uses static variables to save the generated values. If
 * more than one thread needs to call the function and it is likely that they
 * will do it in the same time, use a blocking mechanism such as mutex to
 * prevent interference.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     Perturbated coordinate.
 *
 * @see rabs
 * @see rrand
 * @see rrandn
 * @see saved_len_generator
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
real_t perturbate (size_t i, size_t coordinate)
{
    /* DECLARATION OF STATIC VARIABLES */

    /* Numerical approximation of the mathematical constant pi. */
    static const real_t pi =
        3.1415926535897932384626433832795028841971693993751058209749445923;

    /* Generated length. */
    static real_t r = 0.0;

    /* Generated angle. */
    static real_t phi = 0.0;

    /* DECLARATION OF VARIABLES */

    /* Number of vertices of the original polygon. */
    size_t n;

    /* Coordinates of vertices of the original polygon. */
    const real_t* P;

    /* Length generator. */
    real_t (* generator) (size_t);

    /* Value to compute and return. */
    real_t return_value;

    /* INITIALISATION OF VARIABLES */

    /* Number of vertices of the original polygon. */
    n = 0U;

    /* Coordinates of vertices of the original polygon. */
    P = (real_t*)(NULL);

    /* Length generator. */
    generator = (real_t (*) (size_t))(NULL);

    /* Value to compute and return. */
    return_value = 0.0;

    /* ALGORITHM */

    /* Fetch the saved number of vertices of the original polygon. */
    n = saved_nn_integer(0U);

    /* If the index `i` is strictly less than the number of vertices `n` and
     * the index of the coordinate is 0 or 1, compute the coordinate of the
     * perturbated point. */
    if (i < n && (coordinate == 0U || coordinate == 1U))
    {
        /* Fetch the saved length generator. */
        generator = saved_len_generator((real_t (*) (size_t))(NULL));

        /* Fetch the saved coordinates of vertices. */
        P = saved_polygon((real_t*)(NULL));

        /* If the argument `coordinate` is 0 (if the x-coordinate is to be
         * computed), generate the length and the angle. */
        if (coordinate == 0U)
        {
            /* Generate the length. */
            r = rrandn() * rabs((*generator)(i));

            /* Generate the angle. */
            phi = (real_t)(rrand() * pi);
        }

        /* Set the coordinate to the coordinate of the original point. */
        return_value = *(P + 2U * i + coordinate);

        /* Translate the coordinate for the right value. */
        return_value += (
            (coordinate == 0U) ?
                (r * rcos(phi)) :
                ((coordinate == 1U) ? (r * rsin(phi)) : 0.0)
        );
    }

    /* Return the computed value. */
    return return_value;
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
 * Display polygons that fit inside the square [-1, 1] x [-1, 1].
 *
 * The polygons are displayed by calling the application "display.py".
 *
 * The given array is not mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the command
 * used to call the application for displaying.  If that happens, the returned
 * value may be unexpected.  Exceptions thrown by failed memory allocation in
 * C++ are not caught.
 *
 * @param n
 *     Number of vertices of each polygon.
 *
 * @param P
 *     Array of points of size at least 2 * `n` * `N`.  Each segment of length
 *     2 * `n` of the array (from indices 2 * i including to 2 * (i + 1)
 *     excluding for an index 0 <= i < N) represents a polygon and is organised
 *     as
 *     `{x_0, y_0, x_1, y_1, ..., x_n_minus_1, y_n_minus_1}`, where `x_i` is the
 *     x-coordinate of the `i`-th vertex of the polygon and `y_i` is its
 *     y-coordinate.  Note that the first and the last points inside a segment
 *     are neighbouring vertices of the polygon.
 *
 * @param N
 *     Number of polygons.
 *
 * @return
 *     The value returned by the application used to display the polygons.
 *
 */
int display (size_t n, const real_t* P, size_t N)
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

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Value returned by the application. */
    return_value = 0;

    /* Command. */
    command = (char*)(NULL);

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the number of polygons is 0, break the `do while`-loop. */
        if (!N)
            break;

        /* Allocate the memory for the command. */
        command = (char*)malloc(
            (base_length + N * 16U * n + (N - 1U) * 2U) * sizeof command
        );

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!command)
            break;

        /* Initialise the command to zeros. */
        memset(command, 0, (19U + 16U * n) * sizeof *command);

        /* Print the base command to the beginning of the command. */
        sprintf(command, "%s", base_command);

        /* Iterate over the arrays and print the coordinates to the command. */
        for (j = 0U; j / 2U < n; ++j)
            sprintf(command + base_length + 8U * j, " %7.4f", *(P + j));
        for (i = 1U; i < N; ++i)
        {
            sprintf(command + base_length + 16U * i * n + 2U * (i - 1U), " +");
            for (j = 0U; j / 2U < n; ++j)
                sprintf(
                    command + base_length + i * (16U * n + 2U) + 8U * j,
                    " %7.4f",
                        *(P + 2U * i * n + j)
                );
        }

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
 *     Number of arguments given via the command line.  Should be 3.
 *
 * @param argv
 *     Arguments given via the command line.  Should be an array of length 3
 *     such that the first element is the name (command) of the program, the
 *     second element is the decimal representation of the number of polygons
 *     and the third element is th decimal representation of the number of
 *     vertices.
 *
 * @return
 *     Value 0 if the given arguments were correct and no memory allocation
 *     failed; otherwise a value other than 0.
 *
 */
int main (int argc, char** argv)
{
    /* DECLARATION OF VARIABLES */

    /* Number of polygons. */
    size_t N;

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

    /* Iteration indices. */
    size_t i;
    size_t j;

    /* INITIALISATION OF VARIABLES */

    /* Number of polygons. */
    N = 0U;

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
    j = 0U;

    /* ALGORITHM */

    /* Set the seed for the pseudorandom number generator. */
    srand((unsigned int)time((time_t*)(NULL)));

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the number of command line arguments is not 2, print the error message
     * and exit with a non-zero value. */
    if (!(argc == 3))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "Exactly two additional command line argument (number of polygons"
            "and number of vertices) must be given.\n"
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
    if (!(*argv && *(argv + 1U) && *(argv + 2U)))
    {
        /* Print the error message. */
        fprintf(
            stderr,
            "At least one of the command line arguments is a null-pointer.\n"
        );

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Read the number of polygons from the second command line argument. */
    N = (size_t)atoi(*(argv + 1U));

    /* Read the number of vertices from the third command line argument. */
    n = (size_t)atoi(*(argv + 2U));

    /* If the number of polygons is 0, print the error message and exit with a
     * non-zero value. */
    if (!N)
    {
        /* Print the error message. */
        fprintf(stderr, "Number of polygons must be at least 1.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* If the number of vertices is strictly less than 3, print the error
     * message and exit with a non-zero value. */
    if (n < 3U)
    {
        /* Print the error message. */
        fprintf(stderr, "Number of vertices must be at least 3.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Allocate the memory for the polygons. */
    P = (real_t*)malloc(2U * N * n * sizeof *P);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!P)
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the array `P` to zeros. */
    memset(P, 0, 2U * N * n * sizeof *P);

    /* Allocate memory for the arrays of the edges' lengths and the outer
     * angles. */
    l = (real_t*)malloc(N * n * sizeof *l);
    phi = (real_t*)malloc(N * n * sizeof *phi);

    /* If the memory allocation has failed, print the error message and exit
     * with a non-zero value. */
    if (!(l && phi))
    {
        /* Print the error message. */
        fprintf(stderr, "Memory allocation fail.\n");

        /* Deallocate the memory allocated for the arrays of the edges' lengths
         * and the outer angles. */
        free(l);
        l = (real_t*)(NULL);
        free(phi);
        phi = (real_t*)(NULL);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a non-zero value. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the arrays of the edges' lengths and the outer engles to
     * zeros. */
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *phi);

    /* Save values and functions for generating new values. */
    saved_number(1.0);
    saved_comb_function(rmin);
    saved_len_generator(combiner_length);

    /* Attempt to generate an `n`-gon ATTEMPT_MAX times at the most. */
    for (i = 0U; i < (ATTEMPT_MAX); ++i)
    {
        /* Set the value of number `n_true` to the value of `n`. */
        n_true = n;

        /* Generate an array of `n` points using the `smart_random_polygon`
         * function. */
        smart_random_polygon(
            n,
            P,
            scan_coordinate,
            OUT_ITER_MAX,
            IN_ITER_MAX
        );

        /* Reorder the vertices by flipping the array from index 2 to the
         * end. */
        flip(P + 2U, n - 1U, 2U * sizeof *P);

        /* Simplify and check the array for the polygon.  If the array
         * represents a true `n`-gon, break the `for`-loop. */
        if (simplify_check_polygon(&n_true, P))
            if (n_true == n)
                break;

        /* Clear the memory in the array of points. */
        memset(P, 0, 2U * n * sizeof *P);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* If the original polygon has not been successfully generated, inform the
     * user and exit with a zero-value. */
    if (i >= (ATTEMPT_MAX))
    {
        /* Inform the user. */
        printf("No true %lu-gon found.", n);

        /* Clear the memory in the array of points. */
        memset(P, 0, 2U * N * n * sizeof *P);

        /* Deallocate the memory allocated for the array of points. */
        free(P);
        P = (real_t*)(NULL);

        /* Exit with a zero value. */
        exit(EXIT_SUCCESS);
    }

    /* Normalise the original polygon. */
    normalise_polygon(n, P);

    /* Compute the edges' lengths and the outer angles of the original
     * polygon. */
    describe_polygon(n, P, l, phi);

    /* Save the number of vertices, array of points and arrays of edges' lengths
     * and outer edges. */
    saved_nn_integer(n);
    saved_polygon(P);
    saved_lengths(l);
    saved_angles(phi);

    for (j = 1U; j < N; ++j)
    {
        /* Attempt to generate an `n`-gon ATTEMPT_MAX times at the most by
         * perturbating the first polygon. */
        for (i = 0U; i < (ATTEMPT_MAX); ++i)
        {
            /* Set the value of number `n_true` to the value of `n`. */
            n_true = n;

            /* Generate an array of `n` points using the `random_polygon`
             * function. */
            random_polygon(n, P + 2U * j * n, perturbate);

            /* Reorder the vertices by flipping the array from index 2 to the
             * end. */
            flip(P + 2U * j * n + 2U, n - 1U, 2U * sizeof *P);

            /* Simplify and check the array for the polygon.  If the array
             * represents a true `n`-gon, break the `for`-loop. */
            if (simplify_check_polygon(&n_true, P + 2U * j * n))
                if (n_true == n)
                    break;

            /* Clear the memory in the array of points. */
            memset(P + 2U * j * n, 0, 2U * n * sizeof *P);
        }

        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);

        /* If the perturbated polygon has not been successfully generated,
         * inform the user and exit with a zero-value. */
        if (i >= (ATTEMPT_MAX))
        {
            /* Inform the user. */
            printf("Generation of the perturbated polygon (%lu) failed.\n", j);

            /* Clear the memory in the arrays of the edges' lengths and the
             * outer angles. */
            memset(l, 0, N * n * sizeof *l);
            memset(phi, 0, N * n * sizeof *phi);

            /* Deallocate the memory allocated for the arrays of the edges'
             * lengths and the outer angles. */
            free(l);
            l = (real_t*)(NULL);
            free(phi);
            phi = (real_t*)(NULL);

            /* Clear the memory in the array of points. */
            memset(P, 0, 2U * N * n * sizeof *P);

            /* Deallocate the memory allocated for the array of points. */
            free(P);
            P = (real_t*)(NULL);

            /* Exit with a zero value. */
            exit(EXIT_SUCCESS);
        }

        /* Normalise the perturbated polygon. */
        normalise_polygon(n, P + 2U * j * n);

        /* Compute the edges' lengths and the outer angles of the original
         * polygon. */
        describe_polygon(n, P + 2U * j * n, l + j * n, phi + j * n);
    }

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Display the polygons and print the returned value. */
    return_value = display(n, P, N);
    printf("Returned: %d (0x%03X)\n", return_value, return_value);

    /* Clear the memory in the arrays of the edges' lengths and the outer
     * angles. */
    memset(l, 0, N * n * sizeof *l);
    memset(phi, 0, N * n * sizeof *l);

    /* Deallocate the memory allocated for the arrays of the edges' lengths and
     * the outer angles. */
    free(l);
    l = (real_t*)(NULL);
    free(phi);
    phi = (real_t*)(NULL);

    /* Clear the memory in the array of points. */
    memset(P, 0, 2U * j * n * sizeof *P);

    /* Deallocate the memory allocated for the array of points. */
    free(P);
    P = (real_t*)(NULL);

    /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

    /* Return 0 after successfully running the program. */
    return EXIT_SUCCESS;
}
