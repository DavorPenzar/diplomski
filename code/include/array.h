/**
 * Special functions for arrays.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Check if the header has already been imported. */
#if !(defined(__ARRAY_H__INCLUDED) && (__ARRAY_H__INCLUDED) == 1)

/* Undefine __ARRAY_H__INCLUDED if it has already been defined. */
#if defined(__ARRAY_H__INCLUDED)
#undef __ARRAY_H__INCLUDED
#endif /* __ARRAY_H__INCLUDED */

/* Define __ARRAY_H__INCLUDED as 1. */
#define __ARRAY_H__INCLUDED 1

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

/**
 * Flip the elements in the array.
 *
 * Elements from position `i` and `n` - `i` - 1 in the array will be swapped for
 * all values of `i` in {0, 1, ..., floor(n / 2)}.  More specifically, the array
 * `{a_0, a_1, ..., a_n_minus_2, a_n_minus_1}` is reordered into the array
 * `{a_n_minus_1, a_n_minus_2, ..., a_1, a_0}`.  If the number of elements
 * is odd, the middle element stays in place.
 *
 * The function does not use any additional arrays to reorder the original
 * array.  Only one extra element is used for swapping elements.  The time
 * complexity of the function is O(n).
 *
 * The inverse of the function is the function itself.
 *
 * Obviously, the array is mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the extra
 * element.  If that happens, the array remains unchanged.  Exceptions thrown by
 * failed memory allocation in C++ are not caught.
 *
 * @param a
 *     Pointer to the beginning of the array.
 *
 *     Caution: the array `a` is mutated in the function.
 *
 * @param n
 *     Length of the array (number of elements).
 *
 * @param
 *     Size in bytes of each element in the array `a`.
 *
 */
#if !defined(__cplusplus)
void flip (void* a, size_t n, size_t size)
#else
void flip (void* a, ::size_t n, ::size_t size)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Auxiliary variable. */
    void* aux;

    /* Auxiliary pointer. */
    void* b;

    /* INITIALISATION OF VARIABLES */

    /* Auxiliary variable. */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
    aux = NULL;
#else
    aux = nullptr;
#endif /* __cplusplus */

    /* Auxiliary pointer. */
    b = a;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the array is empty or if any of its elements is empty, break the
         * `do while`-loop. */
        if (!(a && n && size))
            break;

        /* Allocate the memory for the extra element (auxiliary variable). */
#if !defined(__cplusplus)
        aux = malloc(size);
#else
        aux = new unsigned char[size];
#endif /* __cplusplus */

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!aux)
            break;

        /* Initialise the extra element to zero. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Set the pointer `b` to point to the last element of the array. */
#if !defined(__cplusplus)
        b = (unsigned char*)a + (n - 1U) * size;
#else
        b = reinterpret_cast<unsigned char*>(b) + (n - 1U) * size;
#endif /* __cplusplus */

        /* Iterate over the array from both sides until the middle and swap
         * elements from both sides. */
        while (a < b)
        {
            /* Swap the current two elements using the extra element. */
#if !defined(__cplusplus)
            memcpy(aux, b, size);
            memcpy(b, a, size);
            memcpy(a, aux, size);
#else
            ::memcpy(aux, b, size);
            ::memcpy(b, a, size);
            ::memcpy(a, aux, size);
#endif /* __cplusplus */

            /* Increment the pointer `a` to point to the next element, and
             * decrement the pointer `b` to point to the previous element. */
#if !defined(__cplusplus)
            a = (unsigned char*)a + size;
            b = (unsigned char*)b - size;
#else
            a = reinterpret_cast<unsigned char*>(a) + size;
            b = reinterpret_cast<unsigned char*>(b) - size;
#endif /* __cplusplus */
        }

        /* Annul the extra element. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Free the memory for the extra element. */
#if !defined(__cplusplus)
        free(aux);
        aux = NULL;
#else
        delete[] reinterpret_cast<unsigned char*>(aux);
#if (__cplusplus) < 201103L
        aux = NULL;
#else
        aux = nullptr;
#endif /* __cplusplus */
#endif /* __cplusplus */
    }
    while (false);
}

/**
 * Polarise an array on parts with odd and even indices.
 *
 * All the values at odd indices will be pushed to the beginning of the array,
 * and all the values at even indices will be pushed to the end of the array.
 * Relative order of elements at odd (even) indices is preserved.  More
 * specifically, the array
 * `{a_0, b_0, a_1, b_1, ..., a_k_minus_1, b_k_minus_1}` is reordered into the
 * array `{a_0, a_1, ..., a_k_minus_1, b_0, b_1, ..., b_k_minus_1}`.  If there
 * is an additional element `a_k` at the end of the original array, it will end
 * up on position after all other elements `a_i` and before all elements `b_j`.
 *
 * Although the effect of this function can be achieved in linear time
 * complexity using additional arrays, this function uses only one extra
 * element (for swapping) and no additional arrays.  The time complexity of the
 * function is, however, O(n^2).
 *
 * This function is the inverse of the `intertwine` function.
 *
 * Obviously, the array is mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the extra
 * element.  If that happens, the array remains unchanged.  Exceptions thrown by
 * failed memory allocation in C++ are not caught.
 *
 * @param a
 *     Pointer to the beginning of the array.
 *
 *     Caution: the array `a` is mutated in the function.
 *
 * @param n
 *     Length of the array (number of elements).
 *
 * @param
 *     Size in bytes of each element in the array `a`.
 *
 * @see intertwine
 *
 */
#if !defined(__cplusplus)
void polarise (void* a, size_t n, size_t size)
#else
void polarise (void* a, ::size_t n, ::size_t size)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Auxiliary variable. */
    void* aux;

    /* Auxiliary pointers. */
    void* b;
    void* c;

    /* Iteration indices. */
#if !defined(__cplusplus)
    size_t i;
    size_t j;
#else
    ::size_t i;
    ::size_t j;
#endif /* __cplusplus */

    /* INITIALISATION OF VARIABLES */

    /* Auxiliary variable. */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
    aux = NULL;
#else
    aux = nullptr;
#endif /* __cplusplus */

    /* Auxiliary pointers. */
    b = a;
    c = a;

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the array is empty or if any of its elements is empty, break the
         * `do while`-loop. */
        if (!(a && n && size))
            break;

        /* Allocate the memory for the extra element (auxiliary variable). */
#if !defined(__cplusplus)
        aux = malloc(size);
#else
        aux = new unsigned char[size];
#endif /* __cplusplus */

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!aux)
            break;

        /* Initialise the extra element to zero. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Increment the pointer `a` to point to the next element. */
#if !defined(__cplusplus)
        a = (unsigned char*)a + size;
#else
        a = reinterpret_cast<unsigned char*>(a) + size;
#endif /* __cplusplus */

        /* Iterate over the odd positions of the array. */
        for (i = 1U; 2U * i < n; ++i)
        {
            /* Compute the pointer to the `i`-th element after the element
             * pointed at by `a`. */
#if !defined(__cplusplus)
            b = (unsigned char*)a + i * size;
#else
            b = reinterpret_cast<unsigned char*>(a) + i * size;
#endif /* __cplusplus */

            /* Push `i` elements from the `i`-th position after the element
             * pointed at by `a` one place towards the end of the array. */
            for (j = i; j; --j)
            {
                /* Compute the pointer to the predecessor of the element
                 * pointed at by `b`. */
#if !defined(__cplusplus)
                c = (unsigned char*)b - size;
#else
                c = reinterpret_cast<unsigned char*>(b) - size;
#endif /* __cplusplus */

                /* Swap the current element with its predecessor using the extra
                 * element. */
#if !defined(__cplusplus)
                memcpy(aux, b, size);
                memcpy(b, c, size);
                memcpy(c, aux, size);
#else
                ::memcpy(aux, b, size);
                ::memcpy(b, c, size);
                ::memcpy(c, aux, size);
#endif /* __cplusplus */

                /* Set the pointer `b` to the value of `c`. */
                b = c;
            }

            /* Increment the pointer `a` to point to the next element. */
#if !defined(__cplusplus)
            a = (unsigned char*)a + size;
#else
            a = reinterpret_cast<unsigned char*>(a) + size;
#endif /* __cplusplus */
        }

        /* Annul the extra element. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Free the memory for the extra element. */
#if !defined(__cplusplus)
        free(aux);
        aux = NULL;
#else
        delete[] reinterpret_cast<unsigned char*>(aux);
#if (__cplusplus) < 201103L
        aux = NULL;
#else
        aux = nullptr;
#endif /* __cplusplus */
#endif /* __cplusplus */
    }
    while (false);
}

/**
 * Intertwine elements from the first and the second half of an array.
 *
 * The first element in the array (at index 0) stays in place, but all other
 * elements are reordered so that every element from the first half of the array
 * is followed by an element from the other half and vice versa (except for the
 * last element in the resulting array).  Relative order of elements from the
 * first (second) half is preserved.  If the array has an odd number of
 * elements, the middle element is considered to be in the first half.  More
 * specifically, the array
 * `{a_0, a_1, ..., a_k_minus_1, a_k, b_0, b_1, ..., b_k_minus_1}` is reordered
 * into the array `{a_0, b_0, a_1, b_1, ..., a_k_minus_1, b_k_minus_1, a_k}`.
 * If `a_k` is not in the original array, the result will be the same only
 * excluding the element `a_k` in the resulting array as well.
 *
 * Although the effect of this function can be achieved in linear time
 * complexity using additional arrays, this function uses only one extra
 * element (for swapping) and no additional arrays.  The time complexity of the
 * function is, however, O(n^2).
 *
 * This function is the inverse of the `polarise` function.
 *
 * Obviously, the array is mutated in the function.
 *
 * Caution: the function may fail if memory cannot be allocated for the extra
 * element.  If that happens, the array remains unchanged.
 *
 * @param a
 *     Pointer to the beginning of the array.
 *
 *     Caution: the array `a` is mutated in the function.
 *
 * @param n
 *     Length of the array (number of elements).
 *
 * @param
 *     Size in bytes of each element in the array `a`.
 *
 * @see polarise
 *
 */
#if !defined(__cplusplus)
void intertwine (void* a, size_t n, size_t size)
#else
void intertwine (void* a, ::size_t n, ::size_t size)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Auxiliary variable. */
    void* aux;

    /* Auxiliary pointers. */
    void* b;
    void* c;

    /* Iteration indices. */
#if !defined(__cplusplus)
    size_t i;
    size_t j;
#else
    ::size_t i;
    ::size_t j;
#endif /* __cplusplus */

    /* INITIALISATION OF VARIABLES */

    /* Auxiliary variable. */
#if !defined(__cplusplus) || (__cplusplus) < 201103L
    aux = NULL;
#else
    aux = nullptr;
#endif /* __cplusplus */

    /* Auxiliary pointers. */
    b = a;
    c = a;

    /* Iteration indices. */
    i = 0U;
    j = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the array is empty or if any of its elements is empty, break the
         * `do while`-loop. */
        if (!(a && n && size))
            break;

        /* Allocate the memory for the extra element (auxiliary variable). */
#if !defined(__cplusplus)
        aux = malloc(size);
#else
        aux = new unsigned char[size];
#endif /* __cplusplus */

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!aux)
            break;

        /* Initialise the extra element to zero. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Iterate over the first half of the array. */
        for (i = (n - 1U) / 2U; i; --i)
        {
            /* Compute the pointer to the `i`-th element of the array. */
#if !defined(__cplusplus)
            b = (unsigned char*)a + i * size;
#else
            b = reinterpret_cast<unsigned char*>(a) + i * size;
#endif /* __cplusplus */

            /* Push the element pointed at by `b` `i` positions towards the end
             * of the array. */
            for (j = 0U; j < i; ++j)
            {
                /* Compute the pointer to the successor of the element
                 * pointed at by `b`. */
#if !defined(__cplusplus)
                c = (unsigned char*)b + size;
#else
                c = reinterpret_cast<unsigned char*>(b) + size;
#endif /* __cplusplus */

                /* Swap the current element with its successor using the extra
                 * element. */
#if !defined(__cplusplus)
                memcpy(aux, b, size);
                memcpy(b, c, size);
                memcpy(c, aux, size);
#else
                ::memcpy(aux, b, size);
                ::memcpy(b, c, size);
                ::memcpy(c, aux, size);
#endif /* __cplusplus */

                /* Set the pointer `b` to the value of the pointer `c`. */
                b = c;
            }
        }

        /* Annul the extra element. */
#if !defined(__cplusplus)
        memset(aux, 0, size);
#else
        ::memset(aux, 0, size);
#endif /* __cplusplus */

        /* Free the memory for the extra element. */
#if !defined(__cplusplus)
        free(aux);
        aux = NULL;
#else
        delete[] reinterpret_cast<unsigned char*>(aux);
#if (__cplusplus) < 201103L
        aux = NULL;
#else
        aux = nullptr;
#endif /* __cplusplus */
#endif /* __cplusplus */
    }
    while (false);
}

/**
 * Generate the matrix of the unoriented circular representation of an array.
 *
 * The matrix of the unoriented circular representation of the array
 * {a_0, a_1, ..., a_n_minus_1} is the matrix
 *     a_0         a_1         ... a_n_minus_2 a_n_minus_1
 *     a_n_minus_1 a_n_minus_2 ... a_1         a_0
 *     a_1         a_2         ... a_n_minus_1 a_0
 *     a_0         a_n_minus_1 ... a_2         a_1
 *     .           .           .   .           .
 *     .           .            .  .           .
 *     .           .             . .           .
 *     a_n_minus_1 a_0         ... a_n_minus_3 a_n_minus_2
 *     a_n_minus_2 a_n_minus_3 ... a_0         a_n_minus_1
 * i. e. the (2 * i)-th row of the matrix is the array
 * {a_i, a_i_plus_one, ..., a_n_minus_1, a_0, a_1, ..., a_i_minus_one} and the
 * (2 * i + 1)-th row is (2 * i)-th row flipped for all indices i in
 * {0, 1, ..., `n` - 1}.
 *
 * If any of the parameters `nr`, `nc` and `ld` is a null-pointer or if `n` is
 * 0, the function returns the given pointer `A` immediately without allocating
 * memory or changing any of the variables.
 *
 * Caution: if pointers `nr`, `nc` and `ld` do not point to three distinct
 * variables, the result may be unexpected---even the segmentation fault may
 * occur in the function.  This is not checked in the function, it is the user's
 * responsibility to make sure the pointers are distinct.
 *
 * Caution: the function may fail if memory cannot be allocated for the
 * resulting matrix.  If that happens, the null-pointer is returned.  Exceptions
 * thrown by failed memory allocation in C++ are not caught.
 *
 * @param A
 *     Pointer to the first element of the resulting matrix.  The matrix is
 *     filled row-wise and its dimensions are 2 * `n` by `n`.  However, the size
 *     of the array should be at least `*ld` by `n`---see parameter `ld` for the
 *     explanation.  The `i`-th row of the matrix starts at position
 *     `i` * `*ld`, NOT on position `i` * `n`.
 *
 *     If a null-pointer is passed, the memory is dynamically allocated using
 *     the `malloc` function in C and by calling the `new[]` operator in C++.
 *
 * @param nr
 *     Pointer to the variable of the number of rows of the matrix.  The number
 *     is set to 2 * `n`.
 *
 * @param nc
 *     Pointer to the variable of the number of columns of the matrix.  The
 *     number is set to `n`.
 *
 * @param ld
 *     Pointer to the variable of the leading dimension of the matrix.  The
 *     number is set to ceil(`n` / 64) * 64.
 *
 * @param a
 *     Pointer to the beginning of the original array.
 *
 * @param n
 *     Length of the original array (number of elements).
 *
 * @param
 *     Size in bytes of each element in the array `a`.
 *
 */
#if !defined(__cplusplus)
void* build_matrix (
    void* A,
    size_t* nr,
    size_t* nc,
    size_t* ld,
    const void* a,
    size_t n,
    size_t size
)
#else
void* build_matrix (
    void* A,
    ::size_t* nr,
    ::size_t* nc,
    ::size_t* ld,
    const void* a,
    ::size_t n,
    ::size_t size
)
#endif /* __cplusplus */
{
    /* DECLARATION OF VARIABLES */

    /* Auxiliary length variable. */
#if !defined(__cplusplus)
    size_t n_;
#else
    ::size_t n_;
#endif /* __cplusplus */

    /* Iteration indices. */
#if !defined(__cplusplus)
    size_t i;
    size_t j;
    size_t k;
    size_t r;
    size_t s;
#else
    ::size_t i;
    ::size_t j;
    ::size_t k;
    ::size_t r;
    ::size_t s;
#endif /* __cplusplus */

    /* INITIALISATION OF VARIABLES */

    /* Auxiliary length variable. */
    n_ = 0U;

    /* Iteration indices. */
    i = 0U;
    j = 0U;
    k = 0U;
    r = 0U;
    s = 0U;

    /* ALGORITHM */

    /* To avoid using the `goto` command and additional `return` commands, the
     * algorithm is enclosed in a `do while`-loop with a false terminating
     * statement. */
    do
    {
        /* If the array is empty, if any of its elements is empty or if pointers
         * to dimension variables are null-pointers, break the
         * `do while`-loop. */
        if (!(a && n && size && nr && nc && ld))
            break;

        /* Compute `n` - 1 and set the auxiliary length variable `n_` to the
         * result. */
        n_ = n - 1U;

        /* Compute and save dimensions of the matrix. */
        *nr = n << 1U;
        *nc = n;
        *ld = ((n + 63U) >> 6U) << 6U;

        /* Allocate the memory for the array `A` if necessary. */
        if (!A)
#if !defined(__cplusplus)
            A = malloc(*nr * *ld * size);
#else
            A = new unsigned char[*nr * *ld * size];
#endif /* __cplusplus */

        /* If the memory allocation has failed, break the `do while`-loop. */
        if (!A)
            break;

        /* Initialise the matrix to zeros. */
#if !defined(__cplusplus)
        memset(A, 0, *nr * *ld * size);
#else
        ::memset(A, 0, *nr * *ld * size);
#endif /* __cplusplus */

        /* Fill the matrix. */
        for (i = 0U; i < n; ++i)
        {
            /* Set the index `j` to 2 * `i` * `*ld` and index k to
             * `j` + `*ld`. */
            j = (i << 1U) * *ld;
            k = j + *ld;

            /* Fill the (2 * `i`)-th row of the matrix by copying from the
             * original array `a`. */
#if !defined(__cplusplus)
            memcpy(
                (unsigned char*)A + j * size,
                (unsigned char*)a + i * size,
                (n - i) * size
            );
            memcpy((unsigned char*)A + (j + n - i) * size, a, i * size);
#else
            ::memcpy(
                reinterpret_cast<unsigned char*>(A) + j * size,
                reinterpret_cast<unsigned char*>(a) + i * size,
                (n - i) * size
            );
            ::memcpy(A + (j + n - i) * size, a, i * size);
#endif /* __cplusplus */

            /* Fill the (2 * `i` + 1)-th row of the matrix by copying from the
             * (2 * `i`)-th row of the matrix. */
            for (r = 0U, s = n_; r < n; ++r, --s)
#if !defined(__cplusplus)
                memcpy(
                    (unsigned char*)A + (k + r) * size,
                    (unsigned char*)A + (j + s) * size,
                    size
                );
#else
                ::memcpy(
                    reinterpret_cast<unsigned char*>(A) + (k + r) * size,
                    reinterpret_cast<unsigned char*>(A) + (j + s) * size,
                    size
                );
#endif /* __cplusplus */
        }
    }
    while (false);

    /* Return the generated matrix. */
    return A;
}

#endif /* __ARRAY_H__INCLUDED */
