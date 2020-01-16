#!/usr/bin/env bash

##  Compile a C or a C++ code using GCC.
##
##  This file is part of Davor Penzar's master thesis programing.
##
##  Possible usage:
##     ./compile.sh code.c -o program
##  where "code.c" is the path to the source code of the program, and "program"
##  is the path to the resulting runnable file (compiled program).
##
##  The code is compiled with flags:
##      -std=...            --  standard of the language,
##      -pedantic-errors    --  all deviations from the standard result in an
##                              error,
##      -Wall               --  print all warnings,
##      -O                  --  optimisation level 1
##      -pthread            --  compile with pthread.h functions,
##      -lblas              --  link BLAS functions,
##      -llapack            --  link LAPACK functions.
##      -lf2c               --  link after f2c,
##      -lm                 --  link mathematical functions (math.h or cmath),
##      -Iinclude           --  add "include" directory as directory of headers.
##

# Compile the C code.
gcc \
    -std=c89 \
    -pedantic-errors \
    -Wall \
    -O \
    $@ \
    -pthread \
    -lblas \
    -llapack \
    -lf2c \
    -lm \
    -Iinclude
# Possible standards: "-std=c90", "-std=c99", "-std=c11".

# Compile the C++ code.
#g++ \
#    -std=c++98 \
#    -pedantic-errors \
#    -Wall \
#    -O \
#    $@ \
#    -pthread \
#    -lblas \
#    -llapack \
#    -lf2c \
#    -lm \
#    -Iinclude
# Possible standards: "-std=c++11", "-std=c++14", "-std=c++17", "-std=c++2a".
