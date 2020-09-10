#pragma once
#include "Table.h"

class ProductsTable : public Table {


public:
	ProductsTable() : Table{
		{
			{"BARCODE", 20, TypeHolder<int>{}},
			{"PRICE", 10, TypeHolder<double>{}},
			{"PRODDESC", 50, TypeHolder<std::string>{}},
			{"MFG", 20, TypeHolder<std::string>{}},
			{"SUPPLIER", 20, TypeHolder<std::string>{}},
			{"TAXABLE", 7, TypeHolder<bool>{}},
			{"CATEGORY", 20, TypeHolder<std::string>{}},
			{"SALE_PERCENT", 12, TypeHolder<double>{}},
		}
	} {}
};
