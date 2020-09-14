#pragma once
#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

template <typename T>
class Reader {
 public:
  static std::optional<T> read(const std::string& record) {
    T input;
    std::istringstream recordstream(record);
    recordstream >> input;
    if (recordstream.fail()) {
      return {};
    } else {
      return input;
    }
  }
};

// specialize the template for the special behavior of reading string
template <>
class Reader<std::string> {
 public:
  static std::optional<std::string> read(const std::string& record) {
    std::string_view v = record;
    v.remove_suffix(std::min(v.size() - v.find_last_not_of(" ") - 1, v.size()));

    std::string s(v);
    if (s.empty()) {
      return {};
    } else {
      return std::string(v);
    }
  }
};