#pragma once
#include "../base/Table.hpp"

class Query {
 public:
  static void Query1(const Table& customer_table, const Table& zonecost_table);
  static void Query2(const Table& lineitem_table, const Table& products_table);
};
