hw1 : main.o Query.o Table.o
	g++ -std=c++17 -o hw1.exe main.o Query.o Table.o

main.o : main.cpp
	g++ -std=c++17 -c main.cpp

Query.o : util/Query.cpp util/Query.hpp
	g++ -std=c++17 -c util/Query.cpp

Table.o : base/Table.cpp base/Table.hpp base/Column.hpp base/Data.hpp util/Printer.hpp util/Reader.hpp
	g++ -std=c++17 -c base/Table.cpp

clean:
	rm *.o
	rm *.exe
