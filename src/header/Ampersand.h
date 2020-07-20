#ifndef AMPERSAND_H
#define AMPERSAND_H

#include <iostream>

#include "Connector.h"

class Ampersand : public Connector {
    public:
        Ampersand() {}
        Ampersand(Base* left, Base* right) : Connector(left, right) { }

        bool execute(int in, int out) {
            if(lhs->execute(in, out)) {
                if(rhs->execute(in, out)) 
                    return true;

                else 
                    return false;
            }//if

            else 
                return false;
        }//execute
};

#endif