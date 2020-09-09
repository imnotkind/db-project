#pragma once
#include <map>
#include <fstream>
#include "Column.h"



class Table {
protected:
	std::map<std::string, Column> table; //map key : column name, ordered map for good visualization
public:
	virtual bool load_file(std::ifstream& input_file) = 0;
	virtual ~Table() {

	}
	void show();
};
