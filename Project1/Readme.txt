This program needs C++ 17 in MSVC, C++ 2a in g++

Tested on 
- Ubuntu 20.04.1 LTS
- g++ 9.3.0 (Ubuntu 9.3.0-10ubuntu2)

for g++, I just downloaded the default version by doing `sudo apt update && sudo apt install g++` after first installation of Ubuntu

run `make` to build hw1.exe


-------------------------------------

This code is a fully typed dbms, so it is dependent on the types of columns in the table schema.
If you change the type of input, the code won't work since I manually specified the types of data in the code.
This code uses a lot of templates and some newest features to maximize the native typing minimize code duplication.

clang-format -style=google -i *.cpp *.h