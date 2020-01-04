/**
 * Special functions for triangles as polygons.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Check if the header has already been imported. */
#if !(defined(__TRIANGLE_H__INCLUDED) && (__TRIANGLE_H__INCLUDED) == 1)

/* Undefine __TRIANGLE_H__INCLUDED if it has already been defined. */
#if defined(__TRIANGLE_H__INCLUDED)
#undef __TRIANGLE_H__INCLUDED
#endif /* __TRIANGLE_H__INCLUDED */

/* Define __TRIANGLE_H__INCLUDED as 1. */
#define __TRIANGLE_H__INCLUDED 1

/* Import standard library headers. */

#if !defined(__cplusplus)

#include <stddef.h>
#include <string.h>

#else

#include <cstddef>
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
 * Characterise a triangle.
 *
 * A triangle whose edges are of lengths a, b, c such that a >= c >= b, and
 * of inner angles alpha, beta, gamma such that alpha >= beta >= gamma, is
 * characterised by a point in plane of coordinates
 * (1 / 2 - b * cos(gamma) / a, b * sin(gamma) / a)---the point is the
 * characteristic point of the triangle.  The reason for this is that a triangle
 * whose vertices are (1 / 2, 0), V, (-1 / 2, 0), where V is the characteristic
 * point of the original triangle, is similar to the original triangle.
 *
 * The triangle must be described via the `describe_polygon` function
 * beforehand.  It is assumed that the arrays `l` and `phi` are not changed
 * after the description (by shuffling elements or changing their values).  No
 * memory locations may overlap to avoid unexpected results.
 *
 * If any of the given pointers is a null-pointer, no computation is done---the
 * function returns immediately.
 *
 * @param l
 *     Array of the edges' lengths of size 3.
 *
 * @param phi
 *     Array of the outer angles of size 3.
 *
 * @param x
 *     Memory location to store the x-coordinate of the characteristic point.
 *
 * @param y
 *     Memory location to store the y-coordinate of the characteristic point.
 *
 * @param norm
 *     Is the triangle normed or not.  Set this argument to `true` if the
 *     longest edge is of length 1, or `false` otherwise.  If the triangle has
 *     been standardised via the `standardise_polygon` function, set the
 *     argument to `true`.
 *
 * @see describe_polygon
 * @see standardise_polygon
 *
 */
void char_triangle (
    const real_t* l,
    const real_t* phi,
    real_t* x,
    real_t* y,
    bool norm
)
{
    /* DECLARATION OF VARIABLES */

    /* Lengths of edges. */
    real_t a;
    real_t b;
    real_t c;

    /* Outer angles. */
    real_t alpha;
    real_t beta;
    real_t gamma;

    /* Auxiliary variable. */
    real_t aux;

    /* INITIALISATION OF VARIABLES */

    /* Lengths of edges. */
    a = 0.0;
    b = 0.0;
    c = 0.0;

    /* Outer angles. */
    alpha = 0.0;
    beta = 0.0;
    gamma = 0.0;

    /* Auxiliary variable. */
    aux = 0.0;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If any of the pointers `l`, `phi`, `x` and `y` is a null-pointer,
         * break the `do while`-loop. */
        if (!(l && phi && x && y))
            break;

        /* Initialise coordinates of the characteristic point to (0, 0). */
        *x = 0.0;
        *y = 0.0;

        /* Initialise lengths of edges to given lengths. */
        a = rabs(*l);
        b = rabs(*(l + 1U));
        c = rabs(*(l + 2U));

        /* Initialise outer angles to given angles. */
        alpha = rabs(*(phi + 1U));
        beta = rabs(*(phi + 2U));
        gamma = rabs(*phi);

        /* If `a` < `b`, swap `a` and `b` and swap `alpha` and `beta`. */
        if (a < b)
        {
            /* Swap `a` and `b` using the auxiliary variable. */
            aux = a;
            a = b;
            b = aux;

            /* Swap `alpha` and `beta` using the auxiliary variable. */
            aux = alpha;
            alpha = beta;
            beta = aux;
        }

        /* If `a` < `c`, swap `a` and `c` and swap `alpha` and `gamma`. */
        if (a < c)
        {
            /* Swap `a` and `c` using the auxiliary variable. */
            aux = a;
            a = c;
            c = aux;

            /* Swap `alpha` and `gamma` using the auxiliary variable. */
            aux = alpha;
            alpha = gamma;
            gamma = aux;
        }

        /* If `c` < `b`, swap `b` and `c` and swap `beta` and `gamma`. */
        if (c < b)
        {
            /* Swap `b` and `c` using the auxiliary variable. */
            aux = b;
            b = c;
            c = aux;

            /* Swap `beta` and `gamma` using the auxiliary variable. */
            aux = beta;
            beta = gamma;
            gamma = aux;
        }

        /* If the triangle is not normed, divide `b` by `a` if `a` is not 0. */
        if (!norm)
            if (a)
                b /= a;
            else
                b = 0.0;

        /* Compute and save the coordinates of the characteristic point.  Note
         * that `gamma` represents the OUTER angle, not the INNER angle;
         * therefore cos(pi - `gamma`) == -cos(`gamma`) and
         * sin(pi - `gamma`) == sin(`gamma`) are used. */
        *x = 0.5 + b * rcos(gamma);
        *y = b * rsin(gamma);
    }
    while (false);
}

#endif /* __TRIANGLE_H_INCLUDED */
