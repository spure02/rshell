#ifndef APPEND_H
#define APPEND_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>

#include "Connector.h"

class Append : public Connector {
    public:
        Append() {}
        Append(Base* left, Base* right) : Connector(left, right) { }
        bool execute(int in, int out) {
            std::string file;

            //catch a redirection error here for rhs
            if(!rhs->getInfo(file)) {
                std::cout << "redirection error" << std::endl;
                return false;
            }//if

            //open the file here
            out = open(file.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IREAD | 
                    S_IWRITE | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
            
            return lhs->execute(0, out);
        }//execute
};

#endif