#!/bin/bash
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format -style=google -i {} \;
