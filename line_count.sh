#!/bin/bash

CppLineCount=$(find . -type f -name *.cpp | xargs wc -l | tail -1 | awk '{print $1}')
headerLineCount=$(find . -type f -name *.h | xargs wc -l | tail -1 | awk '{print $1}')

echo "There are $CppLineCount lines of cpp, and $headerLineCount in header files."
