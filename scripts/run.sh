#!/bin/bash

find ./build -name "*.gcda" -type f -delete &> /dev/null
find ./build -name "*.gcno" -type f -delete &> /dev/null
rm -f ./build/test/samarium_tests

./scripts/build.sh
~/bin/tryrun ./build/test/samarium_tests
