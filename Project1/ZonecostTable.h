#pragma once
#include "Table.h"

class ZonecostTable : public Table {


public:
	ZonecostTable() : Table{
		{
			{"ZONEID", 6, TypeHolder<int>{}},
			{"ZONEDESC", 20, TypeHolder<std::string>{}},
			{"PRICE", 7, TypeHolder<double>{}},
		}
	} {}
};