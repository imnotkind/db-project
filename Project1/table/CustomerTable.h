#pragma once
#include "../base/Table.h"

class CustomerTable : public Table {


public:
	CustomerTable() : Table{ 
		{
			{"UNAME", 20, TypeHolder<std::string>{}},
			{"PASSWD", 20, TypeHolder<std::string>{}},
			{"LNAME", 20, TypeHolder<std::string>{}},
			{"FNAME", 20, TypeHolder<std::string>{}},
			{"ADDR", 50, TypeHolder<std::string>{}},
			{"ZONE", 6, TypeHolder<int>{}},
			{"SEX", 3, TypeHolder<char>{}},
			{"AGE", 6, TypeHolder<int>{}},
			{"LIMIT", 6, TypeHolder<int>{}},
			{"BALANCE", 10, TypeHolder<double>{}},
			{"CREDITCARD", 20, TypeHolder<std::string>{}},
			{"EMAIL", 50, TypeHolder<std::string>{}},
			{"ACTIVE", 6, TypeHolder<bool>{}},
		} 
	} {}
};
