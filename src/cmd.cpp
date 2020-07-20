#include "header/cmd.h"

Cmd::Cmd(std::vector<std::string> vec) : Base(), argVec(vec), success(true) { }

bool Cmd::execute(int in, int out) {
    bool successful = true;
    pid_t child;
    int status;

    //if first argument is "exit" then simply exit the program
    if(argVec[0] == "exit") exit(0);      

    //create the child process, if successful, convert the string
    //vector to a char vector and call execvp on it
    child = fork();
    if(child == 0) {
        std::vector<char*> args;
        for(int i = 0; i < argVec.size(); i++) {
            args.push_back((char*)argVec[i].c_str());
        } //for
        args.push_back(NULL);

        if(dup2(in, 0) == -1) {
            perror("dup2 error for input");
            return false;
        }//if dup2 == -1 for input

        if(dup2(out, 1) == -1) {
            perror("dup2 error for output");
            return false;
        }//if dup2 == -1 for output

        //catches an error here
        if(execvp(args[0], args.data()) == -1) {
            successful = false;
            perror("executable error");
            exit(1);
        }//if execvp == -1
    } //if child == 0

    //otherwise continue to parent
    else if(child < 0) {
        perror("fork error");
        successful = false;
    }//else if child is not 0

    //pause parent
    else {
        if(waitpid(child, &status, 0) < 0) { 
            perror("wait error exists");
            successful = false;
        } //if waitpid
    }//else

    if(WEXITSTATUS(status) > 0) {
        successful = false;
    }//if exit status is not 0

    return successful;
} //execute

//getter function to return the argument for input/output redirection
bool Cmd::getInfo(std::string& file) {
    if(argVec.size() > 1) return false;
    file = argVec[0];
    return true;
}//getInfo 