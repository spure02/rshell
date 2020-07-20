#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <string>

class Parser;

class Shell {
	protected:
		bool running;
		Parser* parShell;

	public:
		Shell();
		~Shell();
		void build();
};

#include "Parser.h"
#endif