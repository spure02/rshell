#ifndef PARSER_H
#define PARSER_H

#include "cmd.h"
#include "Connector.h"
#include "TestCmd.h"

#include <boost/tokenizer.hpp>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace boost;

class Parser {
    protected:
        std::vector<std::string> data; 
        std::vector<std::string> commandArgs;
        std::vector<std::string> connectorVals;
        std::vector<std::string> parList;
        std::vector<Cmd*> commands;
        std::vector<Connector*> connectors;
        std::vector<TestCmd*> testVec;
        bool singleCmd;
        bool presCheck = false;

    public:
        Parser();
        ~Parser();
        void getInput();
        void initialize();
        void parse(std::string userInput);
        bool runCmds();
        void createCmd(std::vector<std::string> vec);
        bool checkForPrec(int openP, int closedP);
        void createPrecedence();
        std::vector<std::string> getData() { return data; }
};

#endif