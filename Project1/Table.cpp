#include "Table.h"
#include "Printer.h"
#include <iostream>
#include <variant>

void Table::show() {

    for (auto const& [column_name, column] : this->table)
    {
        std::cout << column_name << " | ";

        
        std::visit([](auto&& column) {
            using T = std::decay_t<decltype(column)>::Type;
            Printer<T>::print(column.data_list);
         }, column);
        

        std::cout << std::endl;

    }
}