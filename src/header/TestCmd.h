#ifndef TESTCMD_H
#define TESTCMD_H

#include <sys/stat.h>
#include "cmd.h"
#include <string>
#include <vector>

class TestCmd : public Cmd {
    protected:
        std::string filename1;
        std::string flag1;
        std::vector<std::string> argList; 
        bool flagsize;
    
    public: 
        TestCmd(){} 
        TestCmd(std::vector<std::string> vec) : Cmd(), argList(vec) { }
        
        //the execute file checks if the file exists 
        bool execute(int in, int out) {
            std::vector<std::string> args;
            std::string temp;

            for(std::vector<std::string>::iterator it = argList.begin(); it != argList.end(); ++it) {
                if(*it == "[" ){
                    argList.erase(it); 
                }
                if(*it == "]") {
                    argList.erase(it);
                }
                if(*it != "test") {
                    temp = *it;
                    args.push_back(temp);
                }//if
            }//for it

            //set the flag and filename here
            if(args.size() == 2){
                flag1 = args[0];
                filename1 = args[1];
                flagsize = true;
            }//if

            else{
                flagsize = false;
            }//else

            
            if(flagsize == false) {
                std::cout << "ERROR" << std::endl;
                return false;
            }//if the flag is not 2

            char actual = flag1[1]; 
            struct stat buf;

            if(stat(filename1.c_str(), &buf) == 0){
                //S_ISDIR tests if the file is a directory
                if (actual == 'd') {
                    if(S_ISDIR(buf.st_mode)){
                        std::cout << "(True)" << std::endl;
                        return true;
                    }//if

                    else {
                        std::cout << "(False)" << std::endl;
                        return false;
                    }//else
                }//if

                //if the file/directory exists and is a regular file
                else if(actual == 'f') {
                    if(S_ISREG(buf.st_mode)) {
                        std::cout << "(True)" << std::endl;
                        return true;    
                    }//if

                    else {
                        std::cout << "(False)" << std::endl;
                        return false; 
                    }//else
                }//else if

                else if(actual == 'e'|| actual == ' ') {
                        std::cout << "(True)" << std::endl;
                        return true;            
                }//else if

                else {
                    std::cout << "ERROR: User must specify a valid flag or no flag" << std::endl; 
                    return false;
                }//else
            }//if

            else {
                std::cout << "False" << std::endl;
                return false; 
            }//else 
        }//execute  
};

#endif
