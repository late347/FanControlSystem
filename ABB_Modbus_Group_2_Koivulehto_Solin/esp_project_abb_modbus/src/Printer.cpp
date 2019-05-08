/*
 * Printer.cpp
 *
 *  Created on: 25 Jan 2018
 *      Author: Arvi
 */

#include "Printer.h"

Printer::Printer() {
	// TODO Auto-generated constructor stub
    ITM_init();
}

Printer::~Printer() {
	// TODO Auto-generated destructor stub
}

void Printer::print(string input) const {
	string str = string(input); // + "\n";
	ITM_write(str.c_str());
}

//void Printer::print(char const* input) const {
//	string str = string(input); // + "\n";
//	ITM_write(str.c_str());
//}

//template <class T>
//void Printer::print(T input) const {
//	string str = to_string(input); // + "\n";
//	ITM_write(str.c_str());
//}

//template <class T>
//void Printer::operator()(T input, string whitespace) const {
//	//size_t diff = size_t(size) - str.size();
//	//string whitespace = ""; // string(diff, ' ');
//	string str = to_string(input);
//	print(str + whitespace);
//}
//
//
void Printer::operator()(string str, string whitespace) const {
	//size_t diff = size_t(size) - str.size();
	//string whitespace = ""; // string(diff, ' ');
	print(str + whitespace);
}

void Printer::operator()(char const* str, string whitespace) const {
	//size_t diff = size_t(size) - str.size();
	//string whitespace = ""; // string(diff, ' ');
	print(string(str) + whitespace);
}

void Printer::operator()(int input, string whitespace) const {
	//size_t diff = size_t(size) - str.size();
	//string whitespace = ""; // string(diff, ' ');
	string str = to_string(input);
	print(str + whitespace);
}
