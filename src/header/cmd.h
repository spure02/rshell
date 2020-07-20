#ifndef CMD_H
#define CMD_H

#include "Base.h"

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class Parser;

class Cmd : public Base {
    protected:
        std::vector<std::string> argVec;
        bool success;
    
    public:
        Cmd() {}
        Cmd(std::vector<std::string> vec);
        bool execute(int in, int out);
        bool getInfo(std::string& file);
};

#include "Parser.h"
#endif

