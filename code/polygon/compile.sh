#!/bin/bash

##  Compile a C or a C++ code using GCC.
##
##  Possible usage:
##     ./compile code.c -o program
##  where "code.c" is the path to the source code of the program, and "program"
##  is the path to the resulting runnable file (compiled program).
##
##  The code is compiled with flags:
##      -std=...            --  standard of the language,
##      -pedantic-errors    --  all deviations from the standard result in an
##                              error,
##      -Wall               --  print all warnings,
##      -O                  --  optimisation level 1
##      -lm                 --  link mathematical functions (math.h or cmath),
##      -pthread            --  compile with pthread.h functions.
##

# Compile the C code.
gcc -std=c89 -pedantic-errors -Wall -O $@ -lm -pthread
#gcc -std=c90 -pedantic-errors -Wall -O $@ -lm -pthread
#gcc -std=c99 -pedantic-errors -Wall -O $@ -lm -pthread

# Compile the C++ code.
#g++ -std=c++98 -pedantic-errors -Wall -O $@ -lm -pthread
#g++ -std=c++11 -pedantic-errors -Wall -O $@ -lm -pthread
