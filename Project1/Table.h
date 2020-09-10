#pragma once
#include "Column.h"
#include <unordered_map>
#include <fstream>


using ColumnVariant = DataTypeVariant<Column>;

class Table {
protected:
	std::unordered_map<std::string, ColumnVariant> table; //map key : column name
public:
	virtual ~Table() {
	}
	virtual void load_file(std::ifstream& input_file) = 0;
	void show();
	template <typename T>
	void insert_column(ColumnSchema schema) {
		this->table.insert({ schema.name, Column<T>{schema.name, schema.max_len} });
	}

};
