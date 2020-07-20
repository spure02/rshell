#include "header/Shell.h"
#include "header/Parser.h"
#include "header/cmd.h"
#include "header/Semicolon.h"

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	Shell* rshell = new Shell();
	rshell->build();	
} //main

