#include "CustomerTable.h"
#include "ZonecostTable.h"
#include "LineitemTable.h"
#include "ProductsTable.h"
#include "Query.h"
#include <iostream>
#include <fstream>
#include <string>





int main()
{
    //filename needs to be argv

    std::ifstream customer_file("res/customer.txt");
    CustomerTable customer_table;
    customer_table.load_file(customer_file);
    customer_table.show();
    customer_file.close();

    std::cout << std::endl;

    std::ifstream zonecost_file("res/zonecost.txt");
    ZonecostTable zonecost_table;
    zonecost_table.load_file(zonecost_file);
    zonecost_table.show();
    zonecost_file.close();

    std::cout << std::endl;

    std::ifstream lineitem_file("res/lineitem.txt");
    LineitemTable lineitem_table;
    lineitem_table.load_file(lineitem_file);
    lineitem_table.show();
    lineitem_file.close();

    std::cout << std::endl;

    std::ifstream products_file("res/products_a.txt");
    ProductsTable products_table;
    products_table.load_file(products_file);
    products_table.show();
    products_file.close();

    std::cout << std::endl;

    Query::Query1(customer_table, zonecost_table);

    std::cout << std::endl;

    Query::Query2(lineitem_table, products_table);
   





   


}
