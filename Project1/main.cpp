#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <regex>
#include "CustomerTable.h"


int main()
{

    std::ifstream customer_file("customer.txt");
    CustomerTable customer_table;
    customer_table.show();
    //customer_table.load_file(customer_file);
    customer_file.close();

   





   


}
