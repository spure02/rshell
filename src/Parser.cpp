#include "header/Parser.h"
#include "header/TestCmd.h"
#include "header/Semicolon.h"
#include "header/Ampersand.h"
#include "header/dPipe.h"
#include "header/sPipe.h"
#include "header/inputRed.h"
#include "header/outputRed.h"
#include "header/Append.h"

//initialize singleCmd to false because we assume from the start that the user
//will type in more than one command/argument seperated by a connector
Parser::Parser() : singleCmd(false) {}

//empty the object vectors once done
Parser::~Parser() {
    if(commands.size() > 1) {
        for(int i = 0; i < commands.size(); i++) {
            delete commands[i];
        }//for i
    }//if commands

    if(connectors.size() > 1) {
        for(int i = 0; i < connectors.size(); i++) {
            delete connectors[i];
        }//for
    }//if connectors
}//~Parser

//getInput will print the command prompt with the username and hostname,
//then get the user input from the user. the userInput is then sent to
//void parse() so it can be parsed
void Parser::getInput() {
    std::string username(getlogin());
    char hostname[256];
    std::string input;
    gethostname(hostname, 256);

    std::cout << username << "@" << hostname << "$ ";
    std::getline(std::cin, input);

    int open = 0; 
    int closed = 0;

    //this for loop adds a space before and after the opening/closing parentheses
    for(int i = 0; i < input.size(); ++i) {
        if(input[i] == '(') {
            ++open;
            if(input[i + 1] != ' ') {
                input.insert(i + 1, " "); 
            }   
            if(input[i - 1] != ' ') {
                input.insert(i, " ");
                ++i; 
            } 
        }//if
        
        if(input[i] == ')') { 
            ++closed;
            if(input[i + 1] != ' ') {
                input.insert(i + 1, " "); 
            } 
            if(input[i - 1] != ' ') {
                input.insert(i, " ");
                ++i; 
            } 
        }//if
    }//for i

    if(!checkForPrec(open, closed)) {
        std::cout << "Invalid: expected ')'" << std::endl;
        return;
    }//if invalid number of parentheses

    parse(input);
} //getInput

//run the commands in the command vector based on if the user types in a
//single command or multiple commands, and based on the type of connector
bool Parser::runCmds() {
    //handing comments, so if there is a comment just continue
    if(data.size() < 1) return true;

    //handing if user types in exit
    if(data[0] == "exit") {
        exit(0);
        return false;
    }//if

    else {
        if(singleCmd) { //execute if there is only one command
            commands[0]->execute(0, 1);
            singleCmd = false;
        }//if

        //else initialize the necessary objects and execute based on
        //whatever connector is in the connector vector
        else { 
            if(presCheck) { //if precedence exists do this
                createPrecedence();
                //presList[0]->execute();
                //presCheck = false;
            }//if
            else {
                initialize();
                int numConnect = connectors.size();
                connectors[numConnect - 1]->execute(0, 1);
            }//else
        }//else
        
        //empty all the vectors once done
        data.clear();
        commandArgs.clear();
        connectorVals.clear();
        commands.clear();
        connectors.clear();

        return true;
    }//else
}//runCmds

bool Parser::checkForPrec(int openP, int closedP) {
    if(openP > 0 && closedP > 0) {
        if(openP != closedP) {
            presCheck = false;
            return false;
        }//if

        presCheck = true;
        return true;
    }//if

    presCheck = false;
    return true;
}//checkForPrec

/*     parse will separate the userInput by the spaces using the tokenizer
       function from the boost library and push it into a string vector
       called data. data is then searched to separate connectors from
       the commands/arguments and pushes them into seperate string vectors
       called connectorVals and commandArgs, respectively          */
void Parser::parse(std::string userInput) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" "); //set the seperator to a space
        
    //tokenize the userInput based on spaces
    tokenizer tok(userInput, sep);
            
    //push the tokens into a string vector called data.
    for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
        data.push_back(*beg);
    } //for beg

    //to account for opening and closing brackets [ ]
    //replace the [ with "test" and remove the closing one ]
   for(int i = 0; i < data.size(); ++i) { 
        if(data[i] == "[") {
            data[i] = "test";
        }//if

        if(data.back() == "]") {
            data.pop_back();
        }//if for ] at the back

        if(data[i] == "]") {
            data.erase(data.begin() + i);
        }//if for ] anywhere else
     }//for i
    
/*   seperating the connectors from commands and arguments, so this
     pushes the connectors into a string vector called connectors and
     pushes the commands and arguments into a string vector called commandArgs.
     also accounts for the hash(#) symbol, so if a # is found it will
     break out the loop immediately. while seperating, a Cmd object
     is created for whatever is in commandArgs and then it is cleared immediately
     to avoid the right side commands/arguments being appended onto the
     left side commands/arguments             */
    for(int i  = 0; i < data.size(); ++i) {
        //break out the loop if a # is found
        if(data[i] == "#") break;

        //ignore parentheses for now because we will deal with them later
        if(data[i] == "(" || data[i] == ")") {
            continue;
        }//if for (

        if (data[i] == "&&" || data[i] == "||" || data[i] == ";" || data[i] == "|" || 
            data[i] == "<" || data[i] == ">" || data[i] == ">>") {
            createCmd(commandArgs); //creating the left side Cmd object
            connectorVals.push_back(data[i]);
            commandArgs.clear();
        } //if for connectors

        else {
            commandArgs.push_back(data[i]);
        } //else for everything else
    } //for i

    //creating the right side Cmd object
    createCmd(commandArgs);

    //for single commands only
    if(connectorVals.size() < 1) singleCmd = true;
} //parse

//createCmd is a simple function that just creates a Cmd object from the
//passed in vector and pushes it into the commands vector
void Parser::createCmd(std::vector<std::string> vec) {  
    if(vec[0] == "test") {
        commands.push_back(new TestCmd(vec));
    }//if you find test
    
    else   
        commands.push_back(new Cmd(vec));    
}//createCmd


//this function is all of the precedence. it was previously in the bool execute()
//function in Precedence.h but we were linking it wrong so we migrated it here
//in this void function.
void Parser::createPrecedence() {
    std::vector<std::string> argList;
    std::vector<std::string> connectorList;
    std::vector<Base*> bList;
    
    for(int i = 0; i < data.size(); ++i) {
        if(data.at(i) == "(") {
            connectorList.push_back("()");
        }//if there's an open parentheses

        if(data.at(i) != ";" && data.at(i) != "&&" && data.at(i) != "||"
            && data.at(i) != "|" && data.at(i) != "<" && data.at(i) != ">"
            && data.at(i) != ">>" && data.at(i) != "(" && data.at(i) != ")") {
                    argList.push_back(data.at(i));
         }//if there is no connector or pair of parentheses

        else if (data.at(i) == ";" || data.at(i) == "&&" || data.at(i) == "||" 
            || data.at(i) == "|" || data.at(i) == "<" || data.at(i) == ">" 
            || data.at(i) == ">>" ) {
            if(argList.size() > 0) {
                Cmd* cmdList;
                
                if(argList[0] == "test") {
                    cmdList = new TestCmd(argList);
                }//if argList has test

                else {
                    cmdList = new Cmd(argList);
                }//else

                bList.push_back(cmdList);
                argList.clear();
            }//if there's stuff in argList

            if(connectorList.empty() || connectorList.back() == "()") {
                connectorList.push_back(data.at(i));
            }//if it's a connector

            //big conditional for all the connectors
            else {
                if(connectorList.back() == ";") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new Semicolon(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//if there is a ;

                else if(connectorList.back() == "&&") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new Ampersand(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a &&

                else if(connectorList.back() == "||") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new dPipe(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a ||

                else if(connectorList.back() == "|") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new sPipe(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a |

                else if(connectorList.back() == "<") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new inputRed(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a <

                else if(connectorList.back() == ">") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new outputRed(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a >

                else if(connectorList.back() == ">>") {
                    Base* rhs = bList.back();
                    bList.pop_back();
                    Base* lhs = bList.back();
                    bList.pop_back();

                    bList.push_back(new Append(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.push_back(data.at(i));
                }//else if there is a >>
            }//else
        }//if there is a connector

        if(data.at(i) == ")") {
            if(argList.size() > 0) {
                Cmd* cmdList;
                        
                if(argList[0] == "test") {
                    cmdList = new TestCmd(argList);
                }//if there is test
                        
                else {
                    cmdList = new Cmd(argList);
                }//else

                bList.push_back(cmdList);
                argList.clear();
            }//if there's stuff in argList
                    
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();
                    
            if(connectorList.back() == "()") {
                connectorList.pop_back();
                        
                if(connectorList.back() == ";") {
                    bList.push_back(new Semicolon(lhs, rhs));
                    connectorList.pop_back();
                }//if there is a ;
                        
                else if(connectorList.back() == "&&") {
                    bList.push_back(new Ampersand(lhs, rhs));
                    connectorList.pop_back();
                }//else if here is a &&

                else if(connectorList.back() == "||") {
                    bList.push_back(new dPipe(lhs, rhs));
                    connectorList.pop_back();
                }//else if there is a ||

                else if(connectorList.back() == "|") {
                    bList.push_back(new sPipe(lhs, rhs));
                    connectorList.pop_back();
                }//else if there is a |

                else if(connectorList.back() == "<") {
                    bList.push_back(new inputRed(lhs, rhs));
                    connectorList.pop_back();
                }//else if there is a <

                else if(connectorList.back() == ">") {
                    bList.push_back(new outputRed(lhs, rhs));
                    connectorList.pop_back();
                }//else if there is a >

                else if(connectorList.back() == ">>") {
                    bList.push_back(new Append(lhs, rhs));
                    connectorList.pop_back();
                }//else if there is a >
            }//if the back of connectorList is a ()
                    
            else {
                if(connectorList.back() == ";") {
                    bList.push_back(new Semicolon(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();
                }//if the back of connectorList is a ;

                else if(connectorList.back() == "&&") {
                    bList.push_back(new Ampersand(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();
                }//else if the back of connectorList is a &&

                else if(connectorList.back() == "||") {
                    bList.push_back(new dPipe(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();   
                }//else if the back of connectorList is a ||

                else if(connectorList.back() == "|") {
                    bList.push_back(new sPipe(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();   
                }//else if the back of connectorList is a |

                else if(connectorList.back() == "<") {
                    bList.push_back(new inputRed(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();   
                }//else if the back of connectorList is a <

                else if(connectorList.back() == ">") {
                    bList.push_back(new outputRed(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();   
                }//else if the back of connectorList is a >

                else if(connectorList.back() == ">>") {
                    bList.push_back(new outputRed(lhs, rhs));
                    connectorList.pop_back();
                    connectorList.pop_back();   
                }//else if the back of connectorList is a >>
            }//else if it is a connector
        }//if there is a )
    }//for it

    if(bList.size() > 1) {
        if(connectorList.back() == ";") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            Semicolon* semi = new Semicolon(lhs, rhs);
            semi->execute(0, 1);
        }//if the back of connectorList is a ;

        if(connectorList.back() == "&&") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            Ampersand* amp = new Ampersand(lhs, rhs);
            amp->execute(0, 1);
        }//if the back of connectorList is a &&

        if(connectorList.back() == "||") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            dPipe* pipes = new dPipe(lhs, rhs);
            pipes->execute(0, 1);
        }//if the back of connectorList is a ||

        if(connectorList.back() == "|") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            sPipe* spipes = new sPipe(lhs, rhs);
            spipes->execute(0, 1);
        }//if the back of connectorList is a |

        if(connectorList.back() == "<") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            inputRed* inR = new inputRed(lhs, rhs);
            inR->execute(0, 1);
        }//if the back of connectorList is a <

        if(connectorList.back() == ">") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            outputRed* outR = new outputRed(lhs, rhs);
            outR->execute(0, 1);
        }//if the back of connectorList is a >

        if(connectorList.back() == ">>") {
            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            Append* apps = new Append(lhs, rhs);
            apps->execute(0, 1);
        }//if the back of connectorList is a >>
    }//if there's stuff in bList
            
    else {
        if(bList.size() > 0) {
            Cmd* cmdList;

            if(argList[0] == "test") {
                cmdList = new TestCmd(argList);
            }//if we encounter test

            else {
                cmdList = new Cmd(argList);
            }//else

            bList.push_back(cmdList);
            argList.pop_back();

            Base* rhs = bList.back();
            bList.pop_back();
            Base* lhs = bList.back();
            bList.pop_back();

            if(connectorList.back() == ";") {
                bList.push_back(new Semicolon(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a ;

            else if(connectorList.back() == "&&") {
                bList.push_back(new Ampersand(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a &&

            else if(connectorList.back() == "||") {
                bList.push_back(new dPipe(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a ||

            else if(connectorList.back() == "|") {
                bList.push_back(new sPipe(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a |

            else if(connectorList.back() == "<") {
                bList.push_back(new inputRed(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a <

            else if(connectorList.back() == ">") {
                bList.push_back(new outputRed(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a >

            else if(connectorList.back() == ">>") {
                bList.push_back(new Append(lhs, rhs));
                connectorList.pop_back();
            }//if the back of connectorList is a >>
        }//if there is anything still in bList

        bList.back()->execute(0, 1);
    }//else
}//createPrecedence

//initialize() creates the individual connector objects based on the contents
//of the commands vector
void Parser::initialize() {
/*    first 3 if statements need to handine the initial connector first    */
    if(connectorVals[0] == ";") {
        Semicolon* semi = new Semicolon(commands[0], commands[1]);
        connectors.push_back(semi);
    }//if for ;

    if(connectorVals[0] == "&&") {
        Ampersand* amp = new Ampersand(commands[0], commands[1]);
        connectors.push_back(amp);
    }//if for &&

    if(connectorVals[0] == "||") {
        dPipe* pipes = new dPipe(commands[0], commands[1]);
        connectors.push_back(pipes);
    }//if for ||

    if(connectorVals[0] == "|") {
        sPipe* spipes = new sPipe(commands[0], commands[1]);
        connectors.push_back(spipes);
    }//if for |

    if(connectorVals[0] == "<") {
        inputRed* inR = new inputRed(commands[0], commands[1]);
        connectors.push_back(inR);
    }//if for <

    if(connectorVals[0] == ">") {
        outputRed* outR = new outputRed(commands[0], commands[1]);
        connectors.push_back(outR);
    }//if for >

    if(connectorVals[0] == ">>") {
        Append* apps = new Append(commands[0], commands[1]);
        connectors.push_back(apps);
    }//if for >>

/*  if there is more than one connector, the left hand side will always
    be a Connector object and the right hand side will always be
    a Cmd object                                                    */
    int x = 1;
    for(int y = 2; y < commands.size(); ++y) {
        if(connectorVals[x] == ";") {
            Semicolon* semi = new Semicolon(connectors[x - 1], commands[y]);
            connectors.push_back(semi);
        }//if for ;

        if(connectorVals[x] == "&&") {
            Ampersand* amp = new Ampersand(connectors[x - 1], commands[y]);
            connectors.push_back(amp);
        }//if for &&

        if(connectorVals[x] == "||") {
            dPipe* pipes = new dPipe(connectors[x - 1], commands[y]);
            connectors.push_back(pipes);
        }//if for ||

        if(connectorVals[x] == "|") {
            sPipe* spipes = new sPipe(connectors[x - 1], commands[y]);
            connectors.push_back(spipes);
        }//if for |

        if(connectorVals[x] == "<") {
            inputRed* inR = new inputRed(connectors[x - 1], commands[y]);
            connectors.push_back(inR);
        }//if for <

        if(connectorVals[x] == ">") {
            outputRed* outR = new outputRed(connectors[x - 1], commands[y]);
            connectors.push_back(outR);
        }//if for >

        if(connectorVals[x] == ">>") {
            Append* apps = new Append(connectors[x - 1], commands[y]);
            connectors.push_back(apps);
        }//if for >>
        y++;
    }//for x
} //initalize
