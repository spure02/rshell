#ifndef INPUTRED_H
#define INPUTRED_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>

#include "Connector.h"

class inputRed : public Connector {
    public:
        inputRed() {}
        inputRed(Base* left, Base* right) : Connector(left, right) { }
        bool execute(int in, int out) {
            std::string file;

            //catch a redirection error here
            if(!rhs->getInfo(file)) {
                std::cout << "redirection error" << std::endl;
                return false;
            }//if

            //open the file here
            in = open(file.c_str(), O_RDONLY);

            return lhs->execute(in, out);
        }//execute
};

#endif