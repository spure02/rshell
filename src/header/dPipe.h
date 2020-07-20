#ifndef DPIPE_H
#define DPIPE_H

#include <iostream>
#include "Connector.h"

class dPipe : public Connector {
    public:
        dPipe() {}
        dPipe(Base* left, Base* right) : Connector(left, right) {}

        bool execute(int in, int out) {
        	if(!lhs->execute(in, out)) {
        		if(rhs->execute(in, out)) return true;
        		else return false;
        	}//if
            return true;
        }//execute
};

#endif