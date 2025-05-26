#!/bin/bash

FILES=$(find src include -regex '.*\.\(cpp\|hpp\|cc\|hh\)')
for file in $FILES; do
    diff_output=$(clang-format "$file" | diff -u "$file" -)
    if [ -n "$diff_output" ]; then
        echo "📄 Différences détectées dans : $file"
        echo "$diff_output"
        echo ""
    fi
done