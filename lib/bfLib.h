#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "minmax.h"
#include "bfErrorDef.h"

#define MEMORY_SIZE 2048
#define PROGRAM_SIZE 4096

typedef struct{
	unsigned char memory[MEMORY_SIZE];
	char program[PROGRAM_SIZE];
	Stack programStack;

	int loadedProgramSize;
	int instructionPointer;
	int memoryPointer;

	int programHalted;
	int errorLevel;

}BFProgram;


BFProgram newBFProgram(){
	Stack s = newStack();
	BFProgram bf = 
	{{0},{0},s, // memory, program and programStack
	0,0,0,      // loadedProgramSize, instructionPointer and memoryPointer
	0,0};     // programHalted and errorLevel
	return bf;
}

int loadProgramFromFile(BFProgram* bf, char* fileName);
int verifyProgramLoops(BFProgram* bf);
void printProgramMemory(BFProgram* bf, int startAddress, int endAddress);
void printProgramCode(BFProgram* bf);
void printCurrentInstruction(BFProgram* bf);
void printProgramStack(BFProgram* bf);
void loopStart(BFProgram* bf);
void loopEnd(BFProgram* bf);
void halt(BFProgram* bf, int el);
int programIsHalted(BFProgram* bf);
int getErrorState(BFProgram* bf);


int loadProgramFromFile(BFProgram* bf, char* fileName){
	FILE* file = fopen(fileName,"r");
	char nextCharacter;
	int currentProgramPosition = 0;

	if(file == NULL){
		halt(bf, FILE_LOAD_ERROR);
		return FILE_LOAD_ERROR;
	}

	while((nextCharacter = fgetc(file)) != EOF){

		if(currentProgramPosition >= PROGRAM_SIZE){
			halt(bf,FILE_SIZE_ERROR);
			return FILE_SIZE_ERROR;
		}

		bf->program[currentProgramPosition] = nextCharacter;
		currentProgramPosition++;
	}

	bf->loadedProgramSize = currentProgramPosition;

	return verifyProgramLoops(bf);

}

int verifyProgramLoops(BFProgram* bf){
	int maxNestedSize = 0;
	int minNestedSize = 0;
	int currentNestedSize = 0;
	int i;

	for(i = 0; i < bf->loadedProgramSize; i++){
		if(bf->program[i] == '['){
			currentNestedSize++;
			if(currentNestedSize > maxNestedSize)maxNestedSize = currentNestedSize;
		}else if(bf->program[i] == ']'){
			currentNestedSize--;
			if(currentNestedSize < minNestedSize)minNestedSize = currentNestedSize;
		}
	}

	if(maxNestedSize > STACK_SIZE){
		halt(bf, MAX_STACK_ERROR);
		return MAX_STACK_ERROR;
	}else if(minNestedSize < 0){
		halt(bf, MIN_STACK_ERROR);
		return MIN_STACK_ERROR;
	}else if(currentNestedSize != 0){
		halt(bf, OPEN_LOOP_ERROR);
		return OPEN_LOOP_ERROR;
	}else{
		return NORMAL_EXECUTION;
	}

}

int runNextInstruction(BFProgram* bf){
	if(!programIsHalted(bf)){
		switch(bf->program[bf->instructionPointer]){
			case '+':
				bf->memory[bf->memoryPointer]++;
				break;
			case '-':
				bf->memory[bf->memoryPointer]--;
				break;
			case '>':
				bf->memoryPointer = (bf->memoryPointer+1)%MEMORY_SIZE;
				break;
			case '<':
				bf->memoryPointer = ((bf->memoryPointer-1) + MEMORY_SIZE)%MEMORY_SIZE;
				break;
			case '.':
				printf("%c", bf->memory[bf->memoryPointer]);
				break;
			case ',':
				bf->memory[bf->memoryPointer] = getchar();
				break;
			case '[':
				loopStart(bf);
				break;
			case ']':
				loopEnd(bf);
				break;
		}
		bf->instructionPointer++;
		if(bf->instructionPointer >= bf->loadedProgramSize){
			halt(bf, NORMAL_EXECUTION);
		}
	}
}

void printProgramMemory(BFProgram* bf, int startAddress, int endAddress){
	int i;

	startAddress = max(0, min(startAddress, MEMORY_SIZE-1));
	endAddress = max(0, min(endAddress, MEMORY_SIZE-1));
	
	for(i = startAddress; i <= endAddress; i++){
		if((i != startAddress && (i-startAddress)%8 == 0)){
			printf("\n");
		}
		printf("%03d ", bf->memory[i]);
	}
	printf("\n");

}

void printProgramCode(BFProgram* bf){
	int i;
	for(i = 0; i < bf->loadedProgramSize; i++){
		printf("%c", bf->program[i]);
	}
	printf("\n");
}

void printCurrentInstruction(BFProgram* bf){
	printf("%d:%c\n", bf->instructionPointer, bf->program[bf->instructionPointer]);
	printf("\n");
}

void printProgramStack(BFProgram* bf){
	printStack(&(bf->programStack));
	printf("\n");
}

void loopStart(BFProgram* bf){
	
	int skipLoop = bf->memory[bf->memoryPointer] == 0;
	int previousNestedStackHeight = getStackSize(&(bf->programStack));
	int nestedStackHeight;
	int currentInstruction = bf->instructionPointer;
	if(skipLoop){
		nestedStackHeight = previousNestedStackHeight + 1;
		while(nestedStackHeight > previousNestedStackHeight){
			currentInstruction++;
			if(bf->program[currentInstruction] == '[')nestedStackHeight++;
			else if(bf->program[currentInstruction] == ']')nestedStackHeight--;
		}
		bf->instructionPointer = currentInstruction;
	}else{
		stackPush(&(bf->programStack), currentInstruction);
	}

}

void loopEnd(BFProgram* bf){

	if(bf->memory[bf->memoryPointer] != 0){
		bf->instructionPointer = getStackTop(&(bf->programStack));
	}else{
		stackPop(&(bf->programStack));
	}

}


void halt(BFProgram* bf, int el){
	bf->programHalted = 1;
	bf->errorLevel = el;
}

int programIsHalted(BFProgram* bf){
	return bf->programHalted;
}

int getErrorLevel(BFProgram* bf){
	return bf->errorLevel;
}
