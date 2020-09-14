#include "table/CustomerTable.h"
#include "table/ZonecostTable.h"
#include "table/LineitemTable.h"
#include "table/ProductsTable.h"
#include "util/Query.h"
#include <iostream>
#include <fstream>
#include <string>





int main(int argc, char* argv[])
{

    if (std::string(argv[1]) == "q1") {
        std::ifstream customer_file(argv[2]);
        CustomerTable customer_table;
        customer_table.load_file(customer_file);
        //customer_table.show();
        customer_file.close();


        std::ifstream zonecost_file(argv[3]);
        ZonecostTable zonecost_table;
        zonecost_table.load_file(zonecost_file);
        //zonecost_table.show();
        zonecost_file.close();

        Query::Query1(customer_table, zonecost_table);

    } 
    else if (std::string(argv[1]) == "q2") {
        std::ifstream lineitem_file(argv[2]);
        LineitemTable lineitem_table;
        lineitem_table.load_file(lineitem_file);
        //lineitem_table.show();
        lineitem_file.close();

        std::ifstream products_file(argv[3]);
        ProductsTable products_table;
        products_table.load_file(products_file);
        //products_table.show();
        products_file.close();

        Query::Query2(lineitem_table, products_table);

    }


}
