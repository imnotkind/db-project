#pragma once
#include <optional>
#include <vector>
#include <iomanip>
#include <iostream>

template<typename T>
class Printer {
public:
	static void print(const std::vector<Data<T>>& data_list) {
		for (auto const& typed_data_element : data_list) {
			if (typed_data_element.raw.has_value()) {
				std::cout << typed_data_element.raw.value() << " | ";
			}
			else {
				std::cout << "NULL" << " | ";
			}
		}
	}
};

// specialize the template for the special behavior of reading double
template<>
class Printer<double> {
public:
	static void print(const std::vector<Data<double>>& data_list) {

		for (auto const& typed_data_element : data_list) {
			if (typed_data_element.raw.has_value()) {
				std::cout << std::fixed <<  std::setprecision(2) << typed_data_element.raw.value() << " | ";
			}
			else {
				std::cout << "NULL" << " | ";
			}
		}

	}
};
