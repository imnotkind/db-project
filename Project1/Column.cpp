#include "Column.h"
#include <cassert>


Column::Column(std::string name_, int max_len_, DataType type_) : name{ name_ }, max_len{ max_len_ }, data_list{ ([type_]  {
	switch (type_) {
	case DataType::string:
		return DataList{ std::vector<Data<std::string>>{} };
	case DataType::character:
		return DataList{ std::vector<Data<char>>{} };
	case DataType::boolean:
		return DataList{ std::vector<Data<bool>>{} };
	case DataType::integer:
		return DataList{ std::vector<Data<int>>{} };
	case DataType::decimal:
		return DataList{ std::vector<Data<double>>{} };
	default:
		assert(false);
	}
	
})() }  {}
