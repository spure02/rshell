#ifndef SPIPE_H
#define SPIPE_H

#include <iostream>
#include "Connector.h"

class sPipe : public Connector {
    public:
        sPipe() {}
        sPipe(Base* left, Base* right) : Connector(left, right) { }

        bool execute(int in, int out) {
            int fds[2];

            //catch an error here
            if(pipe(fds) == -1) {
                perror("pipe error");
                return false;
            }//if

            if(!lhs->execute(in, fds[1])) 
                return false;
            close(fds[1]);

            if(!rhs->execute(fds[0], out)) 
                return false;
            close(fds[0]);

            return true;
        }//execute
};

#endif