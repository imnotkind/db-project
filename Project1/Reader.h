#pragma once
#include <string>
#include <string_view>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <optional>

/// A general reader
template<typename T>
class Reader {
public:
	static std::optional<T> read(std::string record) {
		T input;
		std::istringstream recordstream(record);
		recordstream >> input;
		if (recordstream.fail()) {
			//recordstream.clear();
			return {};
		}
		else {
			return input;
		}
		
	}
};

/// Here we specialize the template for the special behavior of reading string
template<>
class Reader<std::string> {
public:
	static std::optional<std::string> read(std::string record) {
		std::string_view v = record;
		//v.remove_prefix(std::min(v.find_first_not_of(" "), v.size()));
		v.remove_suffix(std::min(v.size() - v.find_last_not_of(" ") - 1, v.size()));

		std::string s(v);
		if (s.size() == 0) {
			return {};
		}
		else {
			return std::string(v);
		}
		
	}
};