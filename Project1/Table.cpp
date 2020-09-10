#include "Table.h"
#include "Printer.h"
#include <iostream>
#include <variant>

Table::Table(std::vector<ColumnSchema> table_schema) : TableSchema{ table_schema } {

    for (ColumnSchema schema : table_schema) {
        std::visit([this, schema](auto&& type_holder) {
            using T = std::decay_t<decltype(type_holder)>::Type;
            this->insert_column<T>(schema);
            }, schema.type);
    }
}


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

void Table::load_file(std::ifstream& input_file) {

    std::string line;
    std::getline(input_file, line);
    std::getline(input_file, line);


    while (std::getline(input_file, line)) {

        std::istringstream linestream(line);

        for (ColumnSchema schema : this->TableSchema) {
            std::string record;

            record.resize(schema.max_len);

            linestream.readsome(record.data(), schema.max_len);

            std::visit([this, schema, record](auto&& column) {
                using T = std::decay_t<decltype(column)>::Type;
                column.insert_data<T>(record);
                }, this->table.find(schema.name)->second);

            linestream.readsome(record.data(), 1); //dummy space
        }


    }


}