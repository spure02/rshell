#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <vector>
#include <string>

#include "Base.h" 

class Cmd;

class Connector : public Base {
    protected:
        Base* lhs;
        Base* rhs;

    public:
        Connector() { }
        Connector(Base* left, Base* right) : lhs(left), rhs(right) { }
        bool execute(int in, int out) { return true; }
        bool getInfo(std::string& file) { return false; }
};

#include "cmd.h"
#endif
