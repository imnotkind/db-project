#pragma once
#include "DataType.h"
#include <variant>
#include <optional>



template<typename T>
struct Data {
	std::optional<T> raw; //for null value
};
