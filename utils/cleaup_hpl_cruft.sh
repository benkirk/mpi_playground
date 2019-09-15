#!/bin/bash


# cleanup autoconf derived files

find . -type f \( -name "Makefile.in" \
      -o -name "Makefile" \
      -o -name "*~" \
      -o -name "*.in" \) \
     -print0 | xargs -0 -n 1 echo "rm -f"

echo rm -rf config.\* configure install-sh missing depcomp \*.m4 man www
