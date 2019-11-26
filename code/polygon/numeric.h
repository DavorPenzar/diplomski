/**
 * Specific value types and functions for operations over real numbers.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Check if the header has already been imported. */
#if !(defined(__NUMERIC_H__INCLUDED) && (__NUMERIC_H__INCLUDED) == 1)

/* Undefine __NUMERIC_H__INCLUDED if it has already been defined. */
#if defined(__NUMERIC_H__INCLUDED)
#undef __NUMERIC_H__INCLUDED
#endif /* __NUMERIC_H__INCLUDED */

/* Define __NUMERIC_H__INCLUDED as 1. */
#define __NUMERIC_H__INCLUDED 1

/* Import standard library headers. */

#if !defined(__cplusplus)

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#else

#include <cfloat>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdlib>

#endif /* __cplusplus */

/* Define data types. */

/**
 * Enumeration type for representing signs of real numbers.
 *
 */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
typedef enum
{
    zero    =    0,
    minus   =   -1,
    plus    =    1
}
sign_t;
#else
using   sign_t  =   enum
{
    zero    =    0,
    minus   =   -1,
    plus    =    1
};
#endif /* __cplusplus */

/**
 * Floating point type for representing real numbers.
 *
 */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
typedef double  real_t;
#else
using   real_t  =   double;
#endif /* __cplusplus */

/* Define constants. */

/**
 * Undefined real number (maximal value of `real_t`).
 *
 */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
const   real_t  lambda  =   DBL_MAX;
#else
constexpr   const   real_t  lambda  =   DBL_MAX;
#endif /* __cplusplus */

/**
 * Minimal real number (minimal absolute value of `real_t`).
 *
 */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
const   real_t  epsilon =   DBL_MIN;
#else
constexpr   const   real_t  epsilon =   DBL_MIN;
#endif /* __cplusplus */

/* Define functions. */

/**
 * Decrement in modular arithmetics.
 *
 * @param k
 *     Number to decrement.
 *
 * @param n
 *     Modulus.
 *
 * @return
 *     Number `l` such that `l` < `n` (or `l` == 0 if `n` == 0) and
 *     `l` == `k` - 1 (mod `n`), where "==" denotes the congruence relation.
 *
 */
#if !defined(__cplusplus)
size_t decmod (size_t k, size_t n)
#elif (__cplusplus) < 201103L
inline ::size_t decmod (::size_t k, ::size_t n)
#else
constexpr inline ::size_t decmod (const ::size_t k, const ::size_t n)
#endif /* __cplusplus */
{
    return n ? (k ? ((k - 1U) % n) : (n - 1U)) : 0U;
}

/**
 * Increment in modular arithmetics.
 *
 * @param k
 *     Number to decrement.
 * @param n
 *     Modulus.
 *
 * @return
 *     Number `l` such that `l` < `n` (or `l` == 0 if `n` == 0) and
 *     `l` == `k` + 1 (mod `n`), where "==" denotes the congruence relation.
 *
 */
#if !defined(__cplusplus)
size_t incmod (size_t k, size_t n)
#elif (__cplusplus) < 201103L
inline ::size_t incmod (::size_t k, ::size_t n)
#else
constexpr inline ::size_t incmod (const ::size_t k, const ::size_t n)
#endif /* __cplusplus */
{
    return n ? ((k + 1U) % n) : 0U;
}

/**
 * Get the sign of a real number.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Sign of `x`. If `x` is NaN, 0 is returned.
 *
 */
#if !defined(__cplusplus)
sign_t sign (real_t x)
#elif (__cplusplus) < 201103L
inline sign_t sign (real_t x)
#else
constexpr inline sign_t sign (const real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus) || (__cplusplus) < 201103L
    return (x < 0.0) ? minus : ((0.0 < x) ? plus : zero);
#else
    return (x < 0.0) ?
        sign_t::minus :
        ((0.0 < x) ? sign_t::plus : sign_t::zero);
#endif /* __cplusplus */
}

/**
 * Get the absolute value of a real number.
 *
 * The value is computed using the `fabs` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Absolute value of `x`.
 *
 * @see fabs
 *
 */
#if !defined(__cplusplus)
real_t rabs (real_t x)
#else
inline real_t rabs (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)fabs((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::fabs(x));
#endif /* __cplusplus */
}

/**
 * Get the square root of a real number.
 *
 * The value is computed using the `sqrt` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Square root of `x`.
 *
 * @see sqrt
 *
 */
#if !defined(__cplusplus)
real_t rsqrt (real_t x)
#else
inline real_t rsqrt (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)sqrt((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::sqrt(x));
#endif /* __cplusplus */
}

/**
 * Get the power of a real number.
 *
 * The value is computed using the `pow` function from the standard library.
 *
 * @param base
 *     Real number, base of the power.
 *
 * @param exponent
 *     Real number, exponent of the power.
 *
 * @return
 *     The `exponent` power of the `base`.
 *
 * @see pow
 *
 */
#if !defined(__cplusplus)
real_t rpow (real_t base, real_t exponent)
#else
inline real_t rpow (real_t base, real_t exponent)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (base == 0.0 && 0.0 < exponent) ?
        0.0 :
        (real_t)pow((double)base, (double)exponent);
#else
    return (base == 0.0 && 0.0 < exponent) ?
        0.0 :
        static_cast<real_t>(::pow(base, exponent));
#endif /* __cplusplus */
}

/**
 * Get the exponential function valueated at a real number.
 *
 * The value is computed using the `exp` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     The value of the exponential function valued at `x`.
 *
 * @see exp
 *
 */
#if !defined(__cplusplus)
real_t rexp (real_t x)
#else
inline real_t rexp (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (real_t)exp((double)x);
#else
    return static_cast<real_t>(::exp(x));
#endif /* __cplusplus */
}

/**
 * Get the natural logarithm of a real number.
 *
 * The value is computed using the `log` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Natural logarithm of `x`.
 *
 * @see log
 *
 */
#if !defined(__cplusplus)
real_t rlog (real_t x)
#else
inline real_t rlog (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (real_t)log((double)x);
#else
    return static_cast<real_t>(::log(x));
#endif /* __cplusplus */
}

/**
 * Get the base 10 logarithm of a real number.
 *
 * The value is computed using the `log10` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Base 10 logarithm of `x`.
 *
 * @see log10
 *
 */
#if !defined(__cplusplus)
real_t rlog10 (real_t x)
#else
inline real_t rlog10 (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (real_t)log10((double)x);
#else
    return static_cast<real_t>(::log10(x));
#endif /* __cplusplus */
}

/**
 * Get the sine value of a real number.
 *
 * The value is computed using the `sin` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Sine value of `x`.
 *
 * @see sin
 *
 */
#if !defined(__cplusplus)
real_t rsin (real_t x)
#else
inline real_t rsin (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)sin((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::sin(x));
#endif /* __cplusplus */
}

/**
 * Get the cosine value of a real number.
 *
 * The value is computed using the `cos` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Cosine value of `x`.
 *
 * @see cos
 *
 */
#if !defined(__cplusplus)
real_t rcos (real_t x)
#else
inline real_t rcos (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (real_t)cos((double)x);
#else
    return static_cast<real_t>(::cos(x));
#endif /* __cplusplus */
}

/**
 * Get the tangent value of a real number.
 *
 * The value is computed using the `tan` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Tangent value of `x`.
 *
 * @see tan
 *
 */
#if !defined(__cplusplus)
real_t rtan (real_t x)
#else
inline real_t rtan (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)tan((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::tan(x));
#endif /* __cplusplus */
}

/**
 * Get the arc sine value of a real number.
 *
 * The value is computed using the `asin` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Arc sine value of `x`.
 *
 * @see asin
 *
 */
#if !defined(__cplusplus)
real_t rasin (real_t x)
#else
inline real_t rasin (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)asin((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::asin(x));
#endif /* __cplusplus */
}

/**
 * Get the arc cosine value of a real number.
 *
 * The value is computed using the `acos` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Arc cosine value of `x`.
 *
 * @see acos
 *
 */
#if !defined(__cplusplus)
real_t racos (real_t x)
#else
inline real_t racos (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (real_t)acos((double)x);
#else
    return static_cast<real_t>(::acos(x));
#endif /* __cplusplus */
}

/**
 * Get the arc tangent value of a real number.
 *
 * The value is computed using the `atan` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @return
 *     Arc tangent value of `x`.
 *
 * @see atan
 *
 */
#if !defined(__cplusplus)
real_t ratan (real_t x)
#else
inline real_t ratan (real_t x)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x == 0.0) ? 0.0 : (real_t)atan((double)x);
#else
    return (x == 0.0) ? 0.0 : static_cast<real_t>(::atan(x));
#endif /* __cplusplus */
}

/**
 * Get the arc tangent value of a ratio.
 *
 * The value is computed using the `atan2` function from the standard library.
 *
 * @param x
 *     Real number.
 *
 * @param y
 *     Real number.
 *
 * @return
 *     Arc tangent value of `y` / `x` (if `x` != 0; otherwise pi / 2 or
 *     3 * pi / 2) taking into account the sign of both arguments to determine
 *     the quadrant.
 *
 * @see atan2
 *
 */
#if !defined(__cplusplus)
real_t ratan2 (real_t x, real_t y)
#else
inline real_t ratan2 (real_t x, real_t y)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (y == 0.0 && !(x == 0.0)) ?
        0.0 :
        (real_t)atan2((double)x, (double)y);
#else
    return (x == 0.0 && !(x == 0.0)) ? 0.0 : static_cast<real_t>(::atan2(x, y));
#endif /* __cplusplus */
}

/**
 * Get a pseudorandom real number in the interval [0, 1] following uniform
 * distribution on [0, 1] (U([0, 1])).
 *
 * The pseudorandom value is generated using the `rand` function from the
 * standard library (the returned value is `rand()` / `(RAND_MAX)`).  It is
 * assumed that the `rand` function generates numbers uniformly distributed.
 *
 * @return
 *     Uniformly distributed pseudorandom value from the interval [0, 1].
 *
 * @see rand
 *
 */
#if !defined(__cplusplus)
real_t rrand (void)
#else
inline real_t rrand (void)
#endif /* __cplusplus */
{
    /* Maximal possible value returned by the function `rand`. */
#if !defined(__cplusplus)
    static const real_t rand_max = (real_t)(RAND_MAX);
#else
    static const real_t rand_max = static_cast<real_t>(RAND_MAX);
#endif /* __cplusplus */

    /* Return the random value. */
#if !defined(__cplusplus)
    return (real_t)rand() / rand_max;
#else
    return static_cast<real_t>(::rand()) / rand_max;
#endif /* __cplusplus */
}

/**
 * Get a pseudorandom real number following the standard normal distribution
 * (N(0, 1)).
 *
 * The number is generated using the Box-Muller transformation.  On each odd
 * time of calling the function the function `rrand` is called exactly twice.
 * On each even time of calling the function no other function for generating
 * pseudorandom numbers is called.  As the Box-Muller transformation generates
 * two independent standanormally distributed values, one of the values is
 * returned immediately on each odd time of calling, while the other is cached
 * for the next call to the function.
 *
 * @return
 *     Standardly normally distributed pseudorandom value.
 *
 * @see rrand
 *
 */
real_t rrandn (void)
{
    /* DECLARATION OF VARIABLES */

    /* Numerical approximation of the mathematical constant pi. */
    static const real_t pi = 3.141592653589793;

    /* Flag for indicating if the current call is an odd call. */
    static bool odd = false;

    /* Pseudorandom values. */
    static real_t x = 0.0;
    static real_t y = 0.0;

    /* Value to return. */
    real_t return_value;

    /* INITIALISATION OF VALUES */

    /* Value to return. */
    return_value = 0.0;

    /* If the call is odd, set the flag `odd` to `false` and set the
     * `return_value` to the cached value of `y`. */
    if (odd)
    {
        /* Set the flag `odd` to `false`. */
        odd = false;

        /* Set the return value to the cached value of `y`. */
        return_value = y;
    }
    /* Otherwise set the flag `odd` to `true`, generate next two values and
     * set the `return_value` to one of them. */
    else
    {
        /* Set the flag `odd` to `true`. */
        odd = true;

        /* Generate next two pseudorandom values generated by the `rrand`
         * function and save them to `x` and `y`. */
        x = rrand();
        y = rrand();

        /* Compute sqrt(-2 * log(`x`)) and 2 * pi * `y`, and save the values to
         * `x` and `y` respectively. */
        x = rsqrt(-2.0 * rlog(x));
        y = 2.0 * pi * y;

        /* Compute `x` * cos(`y`) and `x` * sin(`y`), and save the values to `x`
         * and `y` respectively.  In the meantime, save the new value of `x` to
         * `return_value`. */
        return_value = x * rcos(y);
        y = x * rsin(y);
        x = return_value;
    }

    /* Return the `return_value`. */
    return return_value;
}

/**
 * Compare real numbers at given memory adresses.
 *
 * This function is useful for standard library functions such as `qsort`.
 *
 * @param x
 *     Pointer to the first number.
 *
 * @param y
 *     Pointer to the second number.
 *
 * @return
 *    If `x` and `y` are not null-pointers, value -1 if `*x` < `*y`, value 0 if
 *    `*x` == `*y` and value 1 if `*x` > `*y` (where `x` and `y` are viewed as
 *    pointers of type `const real_t*`).
 *
 */
#if !defined(__cplusplus)
int rcompar (const void* x, const void* y)
#elif (__cplusplus) < 201103L
inline int rcompar (const void* x, const void* y)
#else
constexpr inline int rcompar (const void* x, const void* y)
#endif /* __cplusplus */
{
#if !defined(__cplusplus)
    return (x && y) ? (int)sign(*((const real_t*)x) - *((const real_t*)y)) : 0;
#else
    return (x && y) ?
        static_cast<int>(
            sign(
                *(reinterpret_cast<const real_t*>(x)) -
                *(reinterpret_cast<const real_t*>(x))
            )
        ) :
        0;
#endif /* __cplusplus */
}

#endif /* __NUMERIC_H__INCLUDED */
