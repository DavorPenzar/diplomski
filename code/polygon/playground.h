/**
 * Functions to test the library and to "play" with the generators.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Check if the header has already been imported. */
#if !(defined(__PLAYGROUND_H__INCLUDED) && (__PLAYGROUND_H__INCLUDED) == 1)

/* Undefine __PLAYGROUND_H__INCLUDED if it has already been defined. */
#if defined(__PLAYGROUND_H__INCLUDED)
#undef __PLAYGROUND_H__INCLUDED
#endif /* __PLAYGROUND_H__INCLUDED */

/* Define __PLAYGROUND_H__INCLUDED as 1. */
#define __PLAYGROUND_H__INCLUDED 1

/* Import standard library headers. */

#if !defined(__cplusplus)

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#else

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <memory>
#include <new>
#include <stdexcept>

#endif /* __cplusplus */

/* Import package headers. */
#include "boolean.h"
#include "numeric.h"
#include "polygon.h"

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
#if !defined(__cplusplus)
size_t saved_nn_integer (size_t n)
#else
::size_t saved_nn_integer (::size_t n)
#endif /* __cplusplus */
{
    /* Saved non-negative integer. */
#if !defined(__cplusplus)
    static size_t saved_n = 0U;
#else
    static ::size_t saved_n = 0U;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    static const real_t* saved_P = (real_t*)(NULL);
#elif (__cplusplus) < 201103L
    static const real_t* saved_P = reinterpret_cast<real_t*>(NULL);
#else
    static const real_t* saved_P = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    static const real_t* saved_l = (real_t*)(NULL);
#elif (__cplusplus) < 201103L
    static const real_t* saved_l = reinterpret_cast<real_t*>(NULL);
#else
    static const real_t* saved_l = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    static const real_t* saved_phi = (real_t*)(NULL);
#elif (__cplusplus) < 201103L
    static const real_t* saved_phi = reinterpret_cast<real_t*>(NULL);
#else
    static const real_t* saved_phi = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    static real_t (* saved_combiner) (real_t, real_t) =
        (real_t (*) (real_t, real_t))(NULL);
#elif (__cplusplus) < 201103L
    static real_t (* saved_combiner) (real_t, real_t) =
        reinterpret_cast<real_t (*) (real_t, real_t)>(NULL);
#else
    static real_t (* saved_combiner) (real_t, real_t) = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
real_t (* saved_len_generator (real_t (* generator) (size_t))) (size_t)
#else
real_t (* saved_len_generator (real_t (* generator) (::size_t))) (::size_t)
#endif /* __cplusplus */
{
    /* Saved length generator (function). */
#if !defined(__cplusplus)
    static real_t (* saved_generator) (size_t) = (real_t (*) (size_t))(NULL);
#elif (__cplusplus) < 201103L
    static real_t (* saved_generator) (::size_t) =
        reinterpret_cast<real_t (*) (::size_t)>(NULL);
#else
    static real_t (* saved_generator) (::size_t) = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
real_t constant_length (size_t i)
#else
inline real_t constant_length (::size_t i)
#endif /* __cplusplus */
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
#if !defined(__cplusplus)
real_t combiner_length (size_t i)
#else
real_t combiner_length (::size_t i)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Number of edges. */
#if !defined(__cplusplus)
    size_t n;
#else
    ::size_t n;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    l = (real_t*)(NULL);
#elif (__cplusplus) < 201103L
    l = reinterpret_cast<real_t*>(NULL);
#else
    l = nullptr;
#endif /* __cplusplus */

    /* Function for combining two real values. */
#if !defined(__cplusplus)
    combiner = (real_t (*) (real_t, real_t))(NULL);
#elif (__cplusplus) < 201103L
    combiner = reinterpret_cast<real_t (*) (real_t, real_t)>(NULL);
#else
    combiner = nullptr;
#endif /* __cplusplus */

    /* Value to compute and return. */
    return_value = 0.0;

    /* ALGORITHM */

    /* Fetch the saved non-negative integer. */
    n = saved_nn_integer(0U);

    /* Fetch the saved lengths of edges. */
#if !defined(__cplusplus)
    l = saved_lengths((real_t*)(NULL));
#elif (__cplusplus) < 201103L
    l = saved_lengths(reinterpret_cast<real_t*>(NULL));
#else
    l = saved_lengths(nullptr);
#endif /* __cplusplus */

    /* Fetch the saved combining function. */
#if !defined(__cplusplus)
    combiner = saved_comb_function((real_t (*) (real_t, real_t))(NULL));
#elif (__cplusplus) < 201103L
    combiner = saved_comb_function(
        reinterpret_cast<real_t (*) (real_t, real_t)>(NULL)
    );
#else
    combiner = saved_comb_function(nullptr);
#endif /* __cplusplus */

    /* If the index `i` is strictly less than the number of edges `n`, compute
     * the `return_value` by combining the edge at index (`i` - 1) mod `n` and
     * at index `i`. */
    if (i < n)
        return_value = (*combiner)(*(l + decmod(i, n)), *(l + i));

    /* Return the return value. */
    return return_value;
}

/**
 * Copy the existing coordinate.
 *
 * The function returns the coordinate saved to the `saved_polygon` function.
 * The purpose of the function is to pass it as the argument to the
 * `random_polygon` and the `smart_random_polygon` functions.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     The wanted coordinate saved to the `saved_polygon` function if `i` is
 *     strictly less than the number saved to the `saved_nn_integer` function
 *     and `coordinate` is 0 or 1; 0 otherwise.
 *
 * @see rrand
 * @see saved_nn_integer
 * @see saved_polygon
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
#if !defined(__cplusplus)
real_t copy_coordinate(size_t i, size_t coordinate)
#else
inline real_t copy_coordinate(::size_t i, ::size_t coordinate)
#endif /* __cplusplus */
{
#if !defined (__cplusplus)
    return (
        (i < saved_nn_integer(0U) && (coordinate == 0U || coordinate == 1U)) ?
            *(saved_polygon((real_t*)(NULL)) + (i << 1U) + coordinate) :
            0.0
    );
#elif (__cplusplus) < 201103L
    return (
        (i < saved_nn_integer(0U) && (coordinate == 0U || coordinate == 1U)) ?
            *(
                saved_polygon(reinterpret_cast<real_t*>(NULL)) +
                (i << 1U) +
                coordinate
            ) :
            0.0
    );
#else
    return (
        (i < saved_nn_integer(0U) && (coordinate == 0U || coordinate == 1U)) ?
            *(saved_polygon(nullptr) + (i << 1U) + coordinate) :
            0.0
    );
#endif /* __cplusplus */
}

/**
 * Scan the coordinate from the console.
 *
 * The coordinate is queried by printing using the `printf` function and scanned
 * using the `scanf` function.  The C-functions `scanf` and `printf` are used
 * even in C++.
 *
 * The purpose of the function is to pass it as the argument to the
 * `random_polygon` and the `smart_random_polygon` functions.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     Value scanned from the console if `i` is strictly less than the number
 *     saved to the `saved_nn_integer` function and `coordinate` is 0 or 1; 0
 *     otherwise.
 *
 * @see printf
 * @see scanf
 * @see saved_nn_integer
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
#if !defined(__cplusplus)
real_t scan_coordinate (size_t i, size_t coordinate)
#else
real_t scan_coordinate (::size_t i, ::size_t coordinate)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Value to scan and return. */
    real_t return_value;

    /* INITIALISATION OF VARIABLES */

    /* Value to scan and return. */
    return_value = 0.0;

    /* ALGORITHM */

    /* If the index `i` is strictly less than the number of vertices saved to
     * the `saved_nn_integer` function and the index of the coordinate is 0 or
     * 1, scan the coordinate. */
    if (i < saved_nn_integer(0U) && (coordinate == 0U || coordinate == 1U))
    {
        /* Increment the index `i`. */
        ++i;

        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
#if !defined(__cplusplus)
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);
#else
        ::fflush(stdin);
        ::fflush(stdout);
        ::fflush(stderr);
#endif /* __cplusplus */

        /* Print the query to console. */
#if !defined(__cplusplus)
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
#else
        ::printf(
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
#endif /* __cplusplus */

        /* Scan the value from the console. */
        do
        {
            /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
#if !defined(__cplusplus)
            fflush(stdin);
            fflush(stdout);
            fflush(stderr);
#else
            ::fflush(stdin);
            ::fflush(stdout);
            ::fflush(stderr);
#endif /* __cplusplus */
        }
#if !defined(__cplusplus)
        while (!(scanf(" %lf", &return_value) == 1));
#else
        while (!(::scanf(" %lf", &return_value) == 1));
#endif /* __cplusplus */

        /* Flush the `stdin`, `stdout` and the `stderr` buffers. */
#if !defined(__cplusplus)
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);
#else
        ::fflush(stdin);
        ::fflush(stdout);
        ::fflush(stderr);
#endif /* __cplusplus */
    }

    /* Return the scanned value. */
    return return_value;
}

/**
 * Generate a pseudorandom coordinate from the interval [0, 1].
 *
 * The function is actually only a wrapper around the `rrand` function.  The
 * purpose of the function is to pass it as the argument to the `random_polygon`
 * and the `smart_random_polygon` functions.
 *
 * @param i
 *     Index of the point (vertex).
 *
 * @param coordinate
 *     Coordinate of the point (0 being the x-coordinate and 1 being the
 *     y-coordinate).
 *
 * @return
 *     Value returned by calling `rrand()` if `i` is strictly less than the
 *     number saved to the `saved_nn_integer` function and `coordinate` is 0 or
 *     1; 0 otherwise.
 *
 * @see rrand
 * @see saved_nn_integer
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
#if !defined(__cplusplus)
real_t random_coordinate (size_t i, size_t coordinate)
#else
inline real_t random_coordinate (::size_t i, ::size_t coordinate)
#endif /* __cplusplus */
{
    return (
        (i < saved_nn_integer(0U) && (coordinate == 0U || coordinate == 1U)) ?
            rrand() :
            0.0
    );
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
 * `random_polygon` and the `smart_random_polygon` functions.
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
 *     Perturbated coordinate if `i` is strictly less than the number saved to
 *     the `saved_nn_integer` function and `coordinate` is 0 or 1; 0 otherwise.
 *
 * @see rabs
 * @see rrand
 * @see rrandn
 * @see saved_nn_integer
 * @see saved_polygon
 * @see saved_len_generator
 * @see random_polygon
 * @see smart_random_polygon
 *
 */
#if !defined(__cplusplus)
real_t perturbate_coordinate (size_t i, size_t coordinate)
#else
real_t perturbate_coordinate (::size_t i, ::size_t coordinate)
#endif /* __cplusplus */
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
#if !defined(__cplusplus)
    size_t n;
#else
    ::size_t n;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
    P = (real_t*)(NULL);
#elif (__cplusplus) < 201103L
    P = reinterpret_cast<real_t*>(NULL);
#else
    P = nullptr;
#endif /* __cplusplus */

    /* Length generator. */
#if !defined(__cplusplus)
    generator = (real_t (*) (size_t))(NULL);
#elif (__cplusplus) < 201103L
    generator = reinterpret_cast<real_t (*) (size_t)>(NULL);
#else
    generator = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
        generator = saved_len_generator((real_t (*) (size_t))(NULL));
#elif (__cplusplus) < 201103L
        generator = saved_len_generator(
            reinterpret_cast<real_t (*) (size_t)>(NULL)
        );
#else
        generator = saved_len_generator(nullptr);
#endif /* __cplusplus */

        /* Fetch the saved coordinates of vertices. */
#if !defined(__cplusplus)
        P = saved_polygon((real_t*)(NULL));
#elif (__cplusplus) < 201103L
        P = saved_polygon(reinterpret_cast<real_t*>(NULL));
#else
        P = saved_polygon(nullptr);
#endif /* __cplusplus */

        /* If the argument `coordinate` is 0 (if the x-coordinate is to be
         * computed), generate the length and the angle. */
        if (coordinate == 0U)
        {
            /* Generate the length. */
            r = rrandn() * rabs((*generator)(i));

            /* Generate the angle. */
            phi = rrand() * pi;
        }

        /* Set the coordinate to the coordinate of the original point. */
        return_value = *(P + (i << 1U) + coordinate);

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
#if !defined(__cplusplus)
void normalise_polygon (size_t n, real_t* P)
#else
void normalise_polygon (::size_t n, real_t* P)
#endif /* __cplusplus */
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
 * Caution: it is not checked whether or not all the coordinates are in the
 * closed interval [-1, 1].  If any of the coordinates' absolute value is
 * strictly greater than 1, unexpected results may occur (including segmentation
 * fault or exceptions thrown by the displaying application).
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
 *     are neighbouring vertices of the polygon.  All the coordinates should be
 *     in the closed interval [-1, 1].
 *
 * @param N
 *     Number of polygons.
 *
 * @return
 *     The value returned by the application used to display the polygons.
 *
 */
#if !defined(__cplusplus)
int display (size_t n, const real_t* P, size_t N)
#else
int display (::size_t n, const real_t* P, ::size_t N)
#endif /* __cplusplus */
{
    /* The base command for the application used to display the polygon. */
    static const char* base_command = "./display.py";

    /* The delimiter for the polygons. */
    static const char* delim = " +";

    /* The length of the base command. */
#if !defined(__cplusplus)
    static const size_t base_length = 12U;
#else
    static const ::size_t base_length = 12U;
#endif /* __cplusplus */

    /* The length of the delimiter. */
#if !defined(__cplusplus)
    static const size_t delim_length = 2U;
#else
    static const ::size_t delim_length = 2U;
#endif /* __cplusplus */

    /* DECLARATION OF VARIABLES */

    /* Value returned by the application. */
    int return_value;

    /* Command. */
    char* command;

    /* Iteration indices. */
#if !defined(__cplusplus)
    size_t i;
    size_t j;
#else
    ::size_t i;
    ::size_t j;
#endif /* __cplusplus */

    /* INITIALISATION OF VARIABLES */

    /* Value returned by the application. */
    return_value = 0;

    /* Command. */
#if !defined(__cplusplus)
    command = (char*)(NULL);
#elif (__cplusplus) < 201103L
    command = reinterpret_cast<char*>(NULL);
#else
    command = nullptr;
#endif /* __cplusplus */

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
#if !defined(__cplusplus)
        command = (char*)malloc(
            (base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U) *
            sizeof *command
        );
#else
        command = new char[
            base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U
        ];
#endif /* __cplusplus */

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!command)
            break;

        /* Initialise the command to zeros. */
#if !defined(__cplusplus)
        memset(
            command,
            0,
            (base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U) *
                sizeof *command
        );
#else
        ::memset(
            command,
            0,
            (base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U) *
                sizeof *command
        );
#endif /* __cplusplus */

        /* Print the base command to the beginning of the command. */
#if !defined(__cplusplus)
        sprintf(command, "%s", base_command);
#else
        ::sprintf(command, "%s", base_command);
#endif /* __cplusplus */

        /* Iterate over the arrays and print the coordinates to the command. */
        for (j = 0U; (j >> 1U) < n; ++j)
#if !defined(__cplusplus)
            sprintf(command + base_length + (j << 3U), " %7.4f", *(P + j));
#else
            ::sprintf(command + base_length + (j << 3U), " %7.4f", *(P + j));
#endif /* __cplusplus */
        for (i = 1U; i < N; ++i)
        {
#if !defined(__cplusplus)
            sprintf(
                command +
                    base_length +
                    ((i * n) << 4U) +
                    (i - 1U) * delim_length,
                "%s",
                    delim
            );
#else
            ::sprintf(
                command +
                    base_length +
                    (i - 1U) * delim_length,
                "%s",
                    delim
            );
#endif /* __cplusplus */
            for (j = 0U; (j >> 1U) < n; ++j)
#if !defined(__cplusplus)
                sprintf(
                    command +
                        base_length +
                        ((i * n) << 4U) +
                        i * delim_length +
                        (j << 3U),
                    " %7.4f",
                        *(P + ((i * n) << 1U) + j)
                );
#else
                ::sprintf(
                    command +
                        base_length +
                        ((i * n) << 4U) +
                        i * delim_length +
                        (j << 3U),
                    " %7.4f",
                        *(P + ((i * n) << 1U) + j)
                );
#endif /* __cplusplus */
        }

        /* Call the command and get the returned value. */
#if !defined(__cplusplus)
        return_value = system(command);
#else
        return_value = ::system(command);
#endif /* __cplusplus */

        /* Clear the memory in the command. */
#if !defined(__cplusplus)
        memset(
            command,
            0,
            (base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U) *
                sizeof *command
        );
#else
        ::memset(
            command,
            0,
            (base_length + ((N * n) << 4U) + (N - 1U) * delim_length + 1U) *
                sizeof *command
        );
#endif /* __cplusplus */

        /* Deallocate the memory allocated for the command. */
#if !defined(__cplusplus)
        free(command);
        command = (char*)(NULL);
#else
        delete[] command;
#if (__cplusplus) < 201103L
        command = reinterpret_cast<char*>(NULL);
#else
        command = nullptr;
#endif /* __cplusplus */
#endif /* __cplusplus */
    }
    while (false);

    /* Return the value returned by the displaying application. */
    return return_value;
}

#endif /* __PLAYGROUND_H__INCLUDED */
