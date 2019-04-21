#include <stdio.h>
#include "lib/bfLib.h"
#include "lib/bfErrorFunc.h"

int execute(BFProgram* bf, char* filename);
void printUsage();

int main(int argc, char* argv[]){
	BFProgram bf = newBFProgram();
	int errorLevel;
	
	if(argc < 2){
		printUsage();
		errorLevel = NO_FILENAME_ERROR;
	}else{
		errorLevel = execute(&bf, argv[1]);
	}

	
	printErrorMessage(errorLevel);
	return errorLevel;
}

int execute(BFProgram* bf, char* filename){

	loadProgramFromFile(bf, filename);
	while(!programIsHalted(bf)){
		runNextInstruction(bf);
	}
	return getErrorLevel(bf);
}

void printUsage(){
	printf("Usage: <path-to-program> <filename>\n");
}
