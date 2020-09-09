#include "Table.h"
#include <iostream>
#include <variant>

void Table::show() {

    for (auto const& [column_name, column] : this->table)
    {
        std::cout << column_name << " ";         // string (key)

        std::visit([](auto&& arg) {
            for (auto const& typed_data_element : arg) {
                if (typed_data_element.raw.has_value()) {
                    std::cout << typed_data_element.raw.value() << " ";
                }
                else {
                    std::cout << "NULL";
                }
            }
         }, column.data_list);

    }
}