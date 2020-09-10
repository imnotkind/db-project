#pragma once
#include "Data.h"
#include "Reader.h"
#include <string>
#include <vector>
#include <variant>
#include <memory>

using TypeHolderVariant = DataTypeVariant<TypeHolder>;

//for data storage and serialization
struct ColumnSchema {
	std::string name;
	int max_len;
	TypeHolderVariant type;
};

template <typename T>
class Column {
	std::string name;
	int max_len;

public:
	using Type = T;
	std::vector<Data<T>> data_list;
	Column(std::string name_, int max_len_) : name { name_ }, max_len{ max_len_ } {}
	template <typename U>
	void insert_data(std::string record) {
		this->data_list.push_back(Data<T>{Reader<T>::read(record)}); // not constructor, list initialization
	}

};

