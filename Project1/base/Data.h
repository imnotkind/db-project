#pragma once
#include <variant>
#include <optional>
#include <string>



template <template <typename> typename W, typename ...Args>
using VariantDecorator = std::variant<W<Args>...>;


template <template <typename> typename W>
using DataTypeVariant = VariantDecorator<W, std::string, char, bool, int, double>;


template <typename T>
struct TypeHolder {
	using Type = T;
};


template <typename T>
struct Data {
	std::optional<T> raw; //for null values
	using Type = T;
};

