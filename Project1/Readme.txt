This program requires C++ 17

Tested on 
- Ubuntu 20.04.1 LTS
- g++ 9.3.0 (Ubuntu 9.3.0-10ubuntu2)

for g++, I just downloaded the default version by doing `sudo apt update && sudo apt install g++` after first installation of Ubuntu 20.04.1 LTS

run `make` to build hw1.exe

the table data text files are in folder `resource`

-------------------------------------

This code is a fully typed dbms, so it is dependent on the types of columns in the table schema.
If you change the type of input, the code won't work since I manually specified the types of data in the code.
This is an example of the schema that i specified in the code.

```c++
class ProductsTable : public Table {
 public:
  ProductsTable()
      : Table{{
            {"BARCODE", 20, TypeHolder<int>{}},
            {"PRICE", 10, TypeHolder<double>{}},
            {"PRODDESC", 50, TypeHolder<std::string>{}},
            {"MFG", 20, TypeHolder<std::string>{}},
            {"SUPPLIER", 20, TypeHolder<std::string>{}},
            {"TAXABLE", 7, TypeHolder<bool>{}},
            {"CATEGORY", 20, TypeHolder<std::string>{}},
            {"SALE_PERCENT", 12, TypeHolder<double>{}},
        }} {}
};
```

This code uses a lot of templates and some newest features to maximize typing features and minimize code duplication.
The code is formatted by clang-format with google style. Check format.sh for the command I used for formatting.
