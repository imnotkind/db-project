#pragma once
#include "Data.h"
#include <string>
#include <vector>
#include <variant>

template<typename... Args>
using VariantDataList = std::variant<std::vector<Data<Args>>...>; //std::variant<std::vector<Data<std::string>>, std::vector<Data<int>>, ..>

using DataList = VariantDataList<std::string, char, bool, int, double>;


//for data storage and serialization
struct ColumnSchema {
	std::string name;
	int max_len;
	DataType type;
};

class Column {
	std::string name;
	int max_len;
public:
	DataList data_list;
	Column( std::string name_, int max_len_, DataType type_);

};

