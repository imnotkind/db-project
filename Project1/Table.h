#pragma once
#include "Column.h"
#include <unordered_map>
#include <fstream>



using ColumnVariant = DataTypeVariant<Column>;

class Table {
protected:
	std::unordered_map<std::string, ColumnVariant> table; //map key : column name
	std::vector<ColumnSchema> TableSchema;

	Table(std::vector<ColumnSchema> table_schema);


public:
	virtual ~Table() {
	}
	
	void load_file(std::ifstream& input_file);
	void show();
	template <typename T>
	void insert_column(ColumnSchema schema) {
		this->table.insert({ schema.name, Column<T>{schema.name, schema.max_len} });
	}

};
