#pragma once
#include "Data.hpp"
#include <string>
#include <vector>
#include <variant>
#include <memory>

//for data storage and serialization
struct ColumnSchema {
	std::string name;
	int max_len;
	DataTypeVariant<TypeHolder> type;
};

template <typename T>
class Column {
	std::string name;
	int max_len;
	std::vector<Data<T>> data_list;

public:
	using Type = T;
	
	Column(std::string name_, int max_len_) : name { name_ }, max_len{ max_len_ } {}
	void insert_data(Data<T> data){
		this->data_list.push_back(data);
	}
	std::vector<Data<T>> get_data() const {
		return data_list;
	}

};

