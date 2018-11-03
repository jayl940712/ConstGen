#!/bin/bash
find . ! -name 'build_configure.sh' -type f | xargs rm
find . -maxdepth 1 -type d ! -name '.' | xargs rm -r
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../../
