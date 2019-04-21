#define STACK_SIZE 256

typedef struct{
	int currentStackSize;
	int stack[STACK_SIZE];
}Stack;

Stack newStack(){
	Stack s = {0,{0}};
	return s;
}

int isStackEmpty(Stack* s){
	return !(s->currentStackSize);
}

int isStackFull(Stack* s){
	return s->currentStackSize == STACK_SIZE;
}

void stackPush(Stack* s, int value){
	if (!isStackFull(s)){
		s->stack[s->currentStackSize] = value;
		s->currentStackSize++;
	}
}

int stackPop(Stack* s){
	int value = 0;
	if(!isStackEmpty(s)){
		value = s->stack[s->currentStackSize-1];
		s->currentStackSize--;
	}
	return value;
}

int getStackTop(Stack* s){
	if(!isStackEmpty(s)){
		return s->stack[s->currentStackSize-1];
	}else{
		return 0;
	}
}

int getStackSize(Stack* s){
	return s->currentStackSize;
}

void printStack(Stack* s){
	int i;
	for(i = 0; i < s->currentStackSize; i++){
		printf("%d:", s->stack[i]);
	}
	printf("\n");
}
