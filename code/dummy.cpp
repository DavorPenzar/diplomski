/**
 * Dummy file to test compiling using C++.
 *
 * This file is part of Davor Penzar's master thesis programing.
 *
 * @author Davor Penzar <davor.penzar@gmail.com>
 * @version 1.0
 * @package polygon
 *
 */

/* Compile with mathematical constants from "math.h". */
#define _USE_MATH_DEFINES   1

/* Use the DGESDD driver in the `svd_polygon` function from "polygon.h"
 * header. */
#define _USE_SVD_DRIVER 1

/* Include standard C library. */
#include <cstddef>
#include <cstdlib>

/* Include standard C++ library. */
#include <ios>
#include <iostream>
#include <string>

/* Include package headers. */
#include "include/array.h"
#include "include/boolean.h"
#include "include/numeric.h"
#include "include/playground.h"
#include "include/polygon.h"

/**
 * The main program.
 *
 * @param argc
 *     Number of arguments given via the command line.
 *
 * @param argv
 *     Arguments given via the command line.
 *
 * @return
 *     Value 0 if the given arguments were correct and no memory allocation
 *     failed; otherwise a value other than 0.
 *
 */
int main (int argc, char** argv)
{
    /* Declare the "Hello, World!" string. */
    const ::std::string hello("Hello, World!");

    /* Print "Hello, World!" to `stdout`. */
    ::std::cout << hello << ::std::endl;

    /* Return a zero value. */
    return EXIT_SUCCESS;
}
