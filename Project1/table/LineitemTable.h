#pragma once
#include "../base/Table.h"

class LineitemTable : public Table {


public:
	LineitemTable() : Table{
		{
			{"UNAME", 20, TypeHolder<std::string>{}},
			{"DATE", 10, TypeHolder<std::string>{}},
			{"TIME", 8, TypeHolder<std::string>{}},
			{"BARCODE", 20, TypeHolder<int>{}},
			{"QUANTITY", 8, TypeHolder<int>{}},
			{"QUANTITY", 9, TypeHolder<double>{}},
		}
	} {}
};
