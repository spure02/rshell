#include "header/Shell.h"
#include "header/cmd.h"

Shell::Shell() : running {false} 
	{ parShell = new Parser(); }

Shell::~Shell() {
	delete parShell;
}

//build() just builds the shell in a while loop, this will be called
//in main to run the shell
void Shell::build() {
    running = true;
	while(running) {
		parShell->getInput();
		running = parShell->runCmds();
	}//while

	exit(0);
}//build

