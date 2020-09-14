#include "Query.hpp"

#include <iostream>
#include <set>
#include <string>

void Query::Query1(const Table& customer_table, const Table& zonecost_table) {
  int idx1, idx2;

  // active
  std::vector<bool> answer1(customer_table.get_row_size(), false);
  Column<bool> customer_column_active =
      std::get<Column<bool>>(customer_table.get_table().find("ACTIVE")->second);
  idx1 = 0;
  for (Data<bool> customer_data_active : customer_column_active.get_data()) {
    if (customer_data_active.raw.has_value()) {
      if (customer_data_active.raw.value() == true) {
        answer1.at(idx1) = true;
      }
    }
    idx1++;
  }

  // live in toronto
  std::vector<bool> answer2(customer_table.get_row_size(), false);
  const Column<int> customer_column_zone =
      std::get<Column<int>>(customer_table.get_table().find("ZONE")->second);
  const Column<int> zonecost_column_zoneid =
      std::get<Column<int>>(zonecost_table.get_table().find("ZONEID")->second);
  const Column<std::string> zonecost_column_zonedesc =
      std::get<Column<std::string>>(
          zonecost_table.get_table().find("ZONEDESC")->second);
  idx1 = 0;
  for (const Data<int> customer_data_zone : customer_column_zone.get_data()) {
    if (customer_data_zone.raw.has_value()) {
      idx2 = 0;
      for (const Data<int> zonecost_data_zoneid :
           zonecost_column_zoneid.get_data()) {
        if (zonecost_data_zoneid.raw.has_value()) {
          if (customer_data_zone.raw.value() ==
              zonecost_data_zoneid.raw.value()) {
            const Data<std::string> zonecost_data_zonedesc =
                zonecost_column_zonedesc.get_data().at(idx2);
            if (zonecost_data_zonedesc.raw.has_value()) {
              if (zonecost_data_zonedesc.raw.value() == "Toronto") {
                answer2.at(idx1) = true;
              }
            }
          }
        }
        idx2++;
      }
    }
    idx1++;
  }

  // output
  idx1 = 0;
  const Column<std::string> customer_column_lname =
      std::get<Column<std::string>>(
          customer_table.get_table().find("LNAME")->second);
  for (const bool a1 : answer1) {
    const bool a2 = answer2.at(idx1);
    if (a1 == true && a2 == true) {
      const Data<std::string> customer_data_lname =
          customer_column_lname.get_data().at(idx1);
      if (customer_data_lname.raw.has_value()) {
        std::cout << customer_data_lname.raw.value();
      }
      std::cout << std::endl;
    }
    idx1++;
  }
}

void Query::Query2(const Table& lineitem_table, const Table& products_table) {
  int idx1, idx2;

  // product that has been purchased by at least two customers
  std::vector<bool> answer1(products_table.get_row_size(), false);
  const Column<int> products_column_barcode =
      std::get<Column<int>>(products_table.get_table().find("BARCODE")->second);
  idx1 = 0;
  for (Data<int> products_data_barcode : products_column_barcode.get_data()) {
    if (products_data_barcode.raw.has_value()) {
      Column<int> lineitem_column_barcode = std::get<Column<int>>(
          lineitem_table.get_table().find("BARCODE")->second);
      Column<std::string> lineitem_column_uname = std::get<Column<std::string>>(
          lineitem_table.get_table().find("UNAME")->second);

      std::set<std::string> set_uname;
      idx2 = 0;
      for (Data<int> lineitem_data_barcode :
           lineitem_column_barcode.get_data()) {
        if (lineitem_data_barcode.raw.has_value()) {
          if (lineitem_data_barcode.raw.value() ==
              products_data_barcode.raw.value()) {
            Data<std::string> lineitem_data_uname =
                lineitem_column_uname.get_data().at(idx2);
            if (lineitem_data_uname.raw.has_value()) {
              set_uname.insert(lineitem_data_uname.raw.value());
            }
          }
        }

        idx2++;
      }

      if (set_uname.size() > 1) {
        answer1.at(idx1) = true;
      }
    }

    idx1++;
  }

  // output
  idx1 = 0;
  Column<std::string> products_column_proddesc = std::get<Column<std::string>>(
      products_table.get_table().find("PRODDESC")->second);
  for (bool a1 : answer1) {
    if (a1 == true) {
      Data<int> products_data_barcode =
          products_column_barcode.get_data().at(idx1);
      Data<std::string> products_data_proddesc =
          products_column_proddesc.get_data().at(idx1);

      if (products_data_barcode.raw.has_value() &&
          products_data_proddesc.raw.has_value()) {
        std::cout << products_data_barcode.raw.value() << "   "
                  << products_data_proddesc.raw.value() << std::endl;
      }
    }
    idx1++;
  }
}
