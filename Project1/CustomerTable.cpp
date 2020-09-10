#include "CustomerTable.h"
#include <iostream>
#include <sstream>
#include <type_traits>


CustomerTable::CustomerTable() {


	for (ColumnSchema schema : this->CustomerTableSchema) {
		std::visit([this, schema](auto&& type_holder) {
			using T = std::decay_t<decltype(type_holder)>::Type;
			this->insert_column<T>(schema);
			}, schema.type);
	}


}



void CustomerTable::load_file(std::ifstream& input_file) {

	std::string line;
	std::getline(input_file, line);
	std::getline(input_file, line);


	while (std::getline(input_file, line)) {

		std::istringstream linestream(line);

		for (ColumnSchema schema : this->CustomerTableSchema) {
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