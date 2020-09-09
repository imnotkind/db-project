#include <iostream>
#include <sstream>
#include "CustomerTable.h"






CustomerTable::CustomerTable() {
	for (auto schema : this->CustomerTableSchema) {
		this->table.insert({ schema.name, Column{schema} });
	}
}

bool CustomerTable::load_file(std::ifstream& input_file) {

	std::string line;


	while (std::getline(input_file, line)) {
		// Output the text from the file
		std::cout << line << std::endl;

		std::stringstream linestream(line);
		std::string word;

		while (linestream >> word) {
			std::cout << word << std::endl;
		}





	}

	return true;

}