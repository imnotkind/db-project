#pragma once
#include "Data.h"
#include "Reader.h"
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
	void insert_data(std::string record) {
		this->data_list.push_back(Data<T>{Reader<T>::read(record)}); // not constructor, list initialization
	}
	std::vector<Data<T>> get_data() const {
		return data_list;
	}

};

