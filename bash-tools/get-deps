#!/bin/bash

# executable file to check
EXEC=$1

# log dependencies to stdout
ldd $EXEC | \
    sed -n 's/\([^ ]*\) => \/\(mingw\|clang\).*/\1/p' | \
    sed 's/^\(\ \|\t\|\n\)\+//g' | \
    sort -bf

#   get full dep list           ldd $EXEC
#   extract important info      sed -n 's/\([^ ]*\) => \/\(mingw\|clang\).*/\1/p'
#   remove leading whitespace   sed 's/^\(\ \|\t\|\n\)\+//g' --
#   alphabetize results         sort -bf

