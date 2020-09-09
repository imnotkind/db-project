#pragma once
#include "Table.h"

class CustomerTable : public Table {

	const std::vector<ColumnSchema> CustomerTableSchema = {
		{"UNAME", 20, DataType::string},
		{"PASSWD", 20, DataType::string},
		{"LNAME", 20, DataType::string},
		{"FNAME", 20, DataType::string},
		{"ADDR", 50, DataType::string},
		{"ZONE", 6, DataType::integer},
		{"SEX", 3, DataType::character},
		{"AGE", 6, DataType::integer},
		{"LIMIT", 6, DataType::integer},
		{"BALANCE", 10, DataType::decimal},
		{"CREDITCARD", 20, DataType::string},
		{"EMAIL", 50, DataType::string},
		{"ACTIVE", 6, DataType::boolean},
	};

public:
	CustomerTable();
	bool load_file(std::ifstream& input_file);
};
