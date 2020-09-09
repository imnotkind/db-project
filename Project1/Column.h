#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Data.h"
#include "type_holder.h"
#include <variant>
#include <type_traits>

// Have you heard of `HKT`? If not, STUDY.
template<template<typename> typename W, typename... Args>
using VariantDecorator = std::variant<W<Args>...>;

template <template<typename> typename W>
using VariantDecoratorDecorator = VariantDecorator<W, std::string, char, bool, int, double>;

template<typename T>
using VectorData = std::vector<Data<T>>;



using DataList = VariantDecoratorDecorator<VectorData>; //std::variant<std::vector<Data<std::string>>, std::vector<Data<int>>, ..>



//for data storage and serialization
struct ColumnSchema {
	std::string name;
	int max_len;
	VariantDecoratorDecorator<TypeHolder> type;
};

class Column {

	std::string name;
	const int max_len;
	

public:

	DataList data_list;

	//Column(DataType type, std::string name_, int max_len_);

	Column(ColumnSchema schema) : name{ schema.name }, max_len{ schema.max_len }, data_list{ std::visit([](auto&& arg) {
			return DataList{std::vector<Data<std::decay_t<decltype(arg)>::Type>>{}};
		}, schema.type) } {
	}
};

