#pragma once
#include "Column.h"
#include <unordered_map>
#include <fstream>

class Table {
	friend class Query;

protected:
	int row_size = 0;
	std::unordered_map<std::string, DataTypeVariant<Column>> table; //map key : column name
	std::vector<ColumnSchema> table_schema;

	Table(std::vector<ColumnSchema> table_schema_);



public:
	virtual ~Table() {
	}
	
	void load_file(std::ifstream& input_file);
	void show();
	template <typename T>
	void insert_column(ColumnSchema schema) {
		this->table.insert({ schema.name, Column<T>{schema.name, schema.max_len} });
		row_size++;
	}

};


class Query {
public:
	static void Query1(const Table& customer_table, const Table& zonecost_table);
	static void Query2(const Table& lineitem_table, const Table& products_table);
};
