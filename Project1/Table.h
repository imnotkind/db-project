#pragma once
#include "Column.h"
#include <unordered_map>
#include <fstream>


class Table {
protected:
	std::unordered_map<std::string, Column> table; //map key : column name, ordered map for good visualization
public:
	virtual bool load_file(std::ifstream& input_file) = 0;
	virtual ~Table() {

	}
	void show();
};
