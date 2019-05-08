/*
 * Printer.h
 *
 *  Created on: 25 Jan 2018
 *      Author: Arvi
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include "ITM_write.h"
#include <string>
using namespace std;

class Printer {
public:
	Printer();
	virtual ~Printer();
	void operator()(string str, string whitespace = "\n") const;
	void operator()(char const* str, string whitespace = "\n") const;
	void operator()(int str, string whitespace = "\n") const;
	//template <class T>
	//void operator()(T input, string whitespace = "\n") const;
private:
	void print(string input) const;
//	void print(char const* input) const;
//	template <class T>
//	void print(T input) const;
};

#endif /* PRINTER_H_ */
