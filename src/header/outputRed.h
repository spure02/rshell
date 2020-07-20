#ifndef OUTPUTRED_H
#define OUTPUTRED_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>

#include "Connector.h"

class outputRed : public Connector {
    public:
        outputRed() {}
        outputRed(Base* left, Base* right) : Connector(left, right) { }
        bool execute(int in, int out) {
            std::string file;

            //catch a redirection error for rhs
            if(!rhs->getInfo(file)) {
                std::cout << "redirection error" << std::endl;
                return false;
            }//if

            //open the file here
            out = open(file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IREAD | 
                    S_IWRITE | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );

            return lhs->execute(in, out);
        }//execute
};

#endif