#include "Table.h"
#include "Printer.h"
#include <iostream>
#include <variant>

Table::Table(std::vector<ColumnSchema> table_schema_) : table_schema{ table_schema_ } {

    for (ColumnSchema schema : this->table_schema) {
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
            Printer<T>::print(column.get_data());
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

        for (ColumnSchema schema : this->table_schema) {
            std::string record;

            record.resize(schema.max_len);

            linestream.readsome(record.data(), schema.max_len);

            std::visit([this, schema, record](auto&& column) {
                using T = std::decay_t<decltype(column)>::Type;
                column.insert_data(record);
                }, this->table.find(schema.name)->second);

            linestream.readsome(record.data(), 1); //dummy space
        }


    }


}


void Query::Query1(const Table& customer_table, const Table& zonecost_table) {
    int idx1, idx2;

    //active
    std::vector<bool> answer1(customer_table.row_size, false);
    Column<bool> column_active = std::get<Column<bool>>(customer_table.table.find("ACTIVE")->second);
    idx1 = 0;
    for (Data<bool> data_active : column_active.get_data()) {
        if (data_active.raw.has_value()) {
            if (data_active.raw.value() == true) {
                answer1.at(idx1) = true;
            }
        }
        idx1++;
    }


    //live in toronto
    std::vector<bool> answer2(customer_table.row_size, false);
    Column<int> column_zone = std::get<Column<int>>(customer_table.table.find("ZONE")->second);
    Column<int> column_zone_id = std::get<Column<int>>(zonecost_table.table.find("ZONEID")->second);
    Column<std::string> column_zone_desc = std::get<Column<std::string>>(zonecost_table.table.find("ZONEDESC")->second);
    idx1 = 0;
    for (Data<int> data_zone : column_zone.get_data()) {
        if (data_zone.raw.has_value()) {
            idx2 = 0;
            for (Data<int> data_zone_id : column_zone_id.get_data()) {
                if (data_zone_id.raw.has_value()) {
                    if (data_zone.raw.value() == data_zone_id.raw.value()) {
                        Data<std::string> data_zone_desc = column_zone_desc.get_data().at(idx2);
                        if (data_zone_desc.raw.has_value()) {
                            if (data_zone_desc.raw.value() == "Toronto") {
                                answer2.at(idx1) = true;
                            }
                        }
                    }
                }
                idx2++;
            }
        }
        idx1++;
    }

    //output
    idx1 = 0;
    Column<std::string> column_lname = std::get<Column<std::string>>(customer_table.table.find("LNAME")->second);
    for (bool x : answer1) {
        bool y = answer2.at(idx1);
        if (x == true && y == true) {
            Data<std::string> v = column_lname.get_data().at(idx1);
            if (v.raw.has_value()) {
                std::cout << v.raw.value();
            }
            std::cout << std::endl;
        }
        idx1++;
    }
    
}

void Query::Query2(const Table& lineitem_table, const Table& products_table) {

}

