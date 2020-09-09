#include "Column.h"
#include <cassert>

/*
Column::Column(DataType type, std::string name_, int max_len_) : name{ name_  }, max_len{ max_len_ }, data_list{ ([type]  {

	if (type == DataType::string) {
		return DataList{std::vector<Data<std::string>>{}};
	}
	else if (type == DataType::character) {
		return DataList{ std::vector<Data<char>>{} };
	}
	else if (type == DataType::boolean) {
		return DataList{ std::vector<Data<bool>>{} };
	}
	else if (type == DataType::integer) {
		return DataList{ std::vector<Data<int>>{} };
	}
	else if (type == DataType::decimal) {
		return DataList{ std::vector<Data<double>>{} };
	}
	else {
		assert(false);
	}
})() }  {
	
}
*/