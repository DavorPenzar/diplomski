/**
 * Type declaration and constants for boolean values.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Check if the header has already been imported. */
#if !(defined(__BOOLEAN_H__INCLUDED) && (__BOOLEAN_H__INCLUDED) == 1)

/* Undefine __BOOLEAN_H__INCLUDED if it has already been defined. */
#if defined(__BOOLEAN_H__INCLUDED)
#undef __BOOLEAN_H__INCLUDED
#endif /* __BOOLEAN_H__INCLUDED */

/* Define __BOOLEAN_H__INCLUDED as 1. */
#define __BOOLEAN_H__INCLUDED 1

/* Import standard library headers. */

#if !defined(__cplusplus)

#include <stddef.h>

#else

#include <cstddef>

#endif /* __cplusplus */

/* Check if boolean type and values have already been defined. */
#if !( \
    defined(__cplusplus) || \
    ( \
        defined(__bool_true_false_are_defined) && \
        __bool_true_false_are_defined == 1 \
    ) \
)

/**
 * Enumeration type for representing boolean values.
 *
 * Do not check conditions like `if (t == true)`, use `if (t)` instead (also use
 * `if (!t)`, not `if (t == false)` or `if (t != true)`).  Not only is it
 * strange in languages supporting a boolean type, but the program then makes
 * two checks instead of one and results may be unexpected.
 *
 */
typedef enum
{
    false   =    0,
    true    =   !0
}
bool;

#endif /* __cplusplus, __bool_true_false_are_defined */

/* Undefine __bool_true_false_are_defined if it has already been defined. */
#if defined(__bool_true_false_are_defined)
#undef __bool_true_false_are_defined
#endif /* __bool_true_false_are_defined */

/* Define __bool_true_false_are_defined as 1. */
#define __bool_true_false_are_defined 1

#endif /* __BOOLEAN_H__INCLUDED */
