#!/bin/bash

# Compile the C code.
gcc -std=c89 -pedantic-errors -Wall -O $@ -lm -pthread
#gcc -std=c90 -pedantic-errors -Wall -O $@ -lm -pthread
#gcc -std=c99 -pedantic-errors -Wall -O $@ -lm -pthread

# Compile the C++ code.
#g++ -std=c++98 -pedantic-errors -Wall -O $@ -lm -pthread
#g++ -std=c++11 -pedantic-errors -Wall -O $@ -lm -pthread
