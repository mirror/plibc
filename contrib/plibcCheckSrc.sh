#!/bin/sh

# Checks source files for function calls that are not directed
# to PlibC

grep -nrf plibcCheckSrc.grep $1/* | egrep \.c:[[:digit:]]+:
