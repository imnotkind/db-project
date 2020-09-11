#pragma once
#include "Column.h"
#include <unordered_map>
#include <fstream>
#include <type_traits>

class Table {
	friend class Query;

protected:
	std::unordered_map<std::string, DataTypeVariant<Column>> table; //map key : column name
	std::vector<ColumnSchema> table_schema;
	int column_size = 0;
	int data_size = 0;

	Table(std::vector<ColumnSchema> table_schema_);



public:
	virtual ~Table() {
	}
	
	void load_file(std::ifstream& input_file);
	void show();
	template <typename T>
	void insert_column(ColumnSchema schema) {
		this->table.insert({ schema.name, Column<T>{schema.name, schema.max_len} });
		(this->column_size)++;
	}
	template <typename T>
	void insert_column_data(std::string column_name, Data<T> data) {
		Column<T>& column = std::get<Column<T>>(this->table.find(column_name)->second);
		column.insert_data(data);
		(this->data_size)++;
	}

	int get_row_size() const{
		return (this->data_size) / (this->column_size);
	}
	int get_column_size() const{
		return (this->column_size);
	}

};


class Query {
public:
	static void Query1(const Table& customer_table, const Table& zonecost_table);
	static void Query2(const Table& lineitem_table, const Table& products_table);
};
