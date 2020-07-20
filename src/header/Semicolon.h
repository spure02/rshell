#ifndef SEMICOLON_H
#define SEMICOLON_H

#include <iostream>

#include "Connector.h"

class Semicolon : public Connector {
	public:
		Semicolon() {}
		Semicolon(Base* left, Base* right) : Connector(left, right) {}
		bool execute(int in, int out) {
			lhs->execute(in, out); 
			if(rhs->execute(in, out)) return true;
			else return false;
		}//execute
};

#endif