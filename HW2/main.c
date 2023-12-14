/*
NAME: Arda Sönmez
ID: 040210357
GCC Version: (MinGW.org GCC-6.3.0-1) 6.3.0 

- I use seperated linked lists for every records
- Also each character of every record is stored in different nodes within the list as required in Homework

For example record = 040160811;Ali Pehlivan;2005
List* l1 = head-->0-->4-->...1-->1-->;-->A-->l-->i--> ...-->a-->n-->;-->2-->0-->0--->5-->NULL

- I implement Stack with linked list as wanted
-- Top of stack represents head of list
-- push add new node to beginning
-- pop deletes head node

- Also stack's used for holding lists
For example if lists are named by l1, l2, l3, l4 
Stack = 
| l4 |   - top of stack = l4
| l3 |	 - every element of stack represents a char linked list which hold records
| l2 |
| l1 |
------

*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

#define SIZEOFID 10
#define SIZEOFYEAR 5

struct char_node{ // Node struct for char list
	char data;
	struct char_node* next;
};

typedef struct{ // Struct for List (Every record will be saved this List char by char 
	struct char_node* head;
}List;

struct stack_node{
	List* list;
	struct stack_node* next;
};
typedef struct {
	struct stack_node* top; 
}Stack;


// Blueprints

//// Helper Function Blueprints
int strtoInt(char*); // I do not want to use built-in functions
					 // Gets char* as parameter and converts char* to int

//// List Function Blueprints
List* CreateList(); // Creates empty list
struct char_node* CreateNode(char); // creates empty list node
void AddToEnd(List*, char); // Adds new node to end of given list
char* GetID(List*); // Gets School ID from given list
char* GetName(List*); // Gets Name from given list
char* GetBirthYear(List*); // Gets Birth Year from given list
void PrintList(List*); // Prints List to CMD (debug only)

//// Stack Function Blueprints
Stack* CreateStack();  // Creates empty Stack
struct stack_node* CreateStackData(List*); // Creates empty Stack node (I implemented Stack with Linked List)
void push(Stack*, List*); // Push given list to top of stack
List* pop(Stack*); // Pops top of stack and returns top list
int isStackEmpty(Stack*); // Returns is stack empty. If empty returns 1
void PrintStack(Stack*); // Prints Stack to CMD (debug only)
Stack* SortStackbyYear(Stack*); // Sorts stack by Birth Year and returns sorted stack
Stack* SortStackByFacultyCode(Stack*); // Sorts stack by Faculty Code and returns sorted stack
//// File Function Blueprints
int loadFromFile(Stack*, char*); // Loads records from file to given stack
void PrintToOutFile(char*, Stack*); // Prints records to given file with wanted format

// Helper Functions
int strtoInt(char* value){
	int len=0;
	int i;
	int result=0;
	char* temp = value;
	while(*temp != '\0'){ // '\0' represents end of string 
		len++; // Every loop len increases by one
		temp = (temp+1); // temp is a pointer of given value, it starts from first char, than we need to add 1 value every loop
		//  example input = 123456 
		//				*temp->1, *(temp+1)->2 ...
	} // Finally this loop gives len of value
	// In ASCII table codes of numbers are ordered ('0'=53, '1'=54 ...)
	// Then, '0' - 'N' gives integer value of N
	// But also we need to multiply every number of value with its place value
	// example input = 123, place value of 1 is 10^(len-1), 2 is 10^(len-2) ... 
	for(i=len-1; i>=0; i--){
		result += (int)(pow(10, i)+0.5) * (*value - '0');
		value = (value+1);
	} // This loop calculetes all place values and sums

	return result; // return integer value
}

// Linked List Functions
List* CreateList(){
	List* newList = (List*)malloc(sizeof(List));
	// We need list in heap, malloc function allocates an empty memory storage from head
	// (List *) part indicates the memory storage data type 
	newList->head = NULL; 
	// We defines list's head as NULL
	// That means list is currently empty
	return newList;
	// returns the address of list
	// if the list wasn't in the heap, we wouldn't be able to return
	// because, function stacks are cleared after return
}

struct char_node* CreateNode(char data){
	struct char_node* newNode = (struct char_node*)malloc(sizeof(struct char_node));
	// Same as new list, only we used the (struct char_node*) type and size of one node
	// Node includes an data (char) and one node pointer

	newNode->data = data;
	// Sets newNode's data as given data parameter
	newNode->next = NULL;
	// next is currently empty
	
	return newNode;	
	// returns newNode's address
}

void AddToEnd(List* l, char data){
	struct char_node* last = l->head; // temp node for travel to end
	struct char_node* newNode = CreateNode(data); // Creates new node with given data
	
	if(l->head == NULL) // If list is currently empty, we need just set newNode as list's head
		l->head = newNode;
	else{
		while(last->next != NULL) // This loops travels to end (at last node, last->next is NULL)
			last = last->next;
		
		last->next = newNode; // Set last->next NULL to newNode address
	}
}
char* GetID(List* li){ // Gets School ID from given list
	struct char_node* currentNode = li->head; // Defines a node for Travel
	char* schoolID = (char *)malloc(SIZEOFID * sizeof(char)); // Allocates memory space from heap with given size
	// Every School ID has same number of digits (9), then I defined const SIZEOFID as 10, +1 for '\0'
	int i;
	for(i=0; i<SIZEOFID; i++){ // Loop repeats 9 times
		schoolID[i] = currentNode->data; // gets currentNode's char and sets schoolID[i]  
		currentNode = currentNode->next; // sets currentNode as next node
	}
	schoolID[SIZEOFID-1] = '\0'; // last char of string must be '\0'
	return schoolID;
}
char* GetName(List* li){ // Gets name from given list
	struct char_node* currentNode = li->head;
	int nameLen =0; // represents lenght of name
	int i;
	while(currentNode->data != ';')
		currentNode = currentNode->next;
	currentNode = currentNode->next;
	// this loop provides travel to right after first ';'
	// beacuse, name of record starts from here
	
	struct char_node* startofNameNode = currentNode;
	// I defined a new node to preserve the node where the name begins
	
	while(currentNode->data != ';'){
		nameLen++;
		currentNode = currentNode->next;
	}// This loop iterates until it reaches the next semicolon, Counts lenght of name

	currentNode = startofNameNode; //I reassigned the currentNode to the beginning of the name
	char* name = (char*)malloc(sizeof(char) * nameLen); // after calculate lenght of name we can allocate memory from heap
	for(i=0; i<nameLen; i++){
		name[i] = currentNode->data;
		currentNode = currentNode->next;
	} // gets name until last char of name
	return name;
}

char* GetBirthYear(List* li){ // Gets Birth Year from given list
	struct char_node* currentNode = li->head;
	char* birthYear = (char *)malloc(SIZEOFYEAR * sizeof(char));
	// Every Birth year has same number of digits (4), then I defined const SIZEOFYEAR as 5, +1 for '\0'
	int i;

	while(currentNode->data != ';')
		currentNode = currentNode->next;

	currentNode = currentNode->next;
	// this loop provides travel to right after first ';'
	while(currentNode->data != ';')
		currentNode = currentNode->next;

	currentNode = currentNode->next;
	// this loop provides travel to right after second ';'

	for(i=0; i<SIZEOFYEAR-1; i++){
		birthYear[i] = currentNode->data;
		currentNode = currentNode->next;
	} // gets year until last char of year
	birthYear[SIZEOFYEAR-1] = '\0'; // last char of string must be '\0'

	return birthYear;
}

void PrintList(List* l){ // prints Linked list for debug purposes
	struct char_node* current = l->head; // current starts from head
	while(current != NULL){
		printf("'%c'-->", current->data); // prints every data with--> ('-->' just for good looking)
		current = current->next; // current is next node in every loop
	}
	printf("NULL"); // prints 'NULL' for show last node->next points 
}

// Stack Functions

// I implemented stack with linked list
Stack* CreateStack(){ // Creates new and empty Stack
	Stack* newStack = (Stack*)malloc(sizeof(Stack)); // Allocates memory from heap
	newStack->top = NULL; // top = null means Stack is currently empty

	return newStack; // returns new and empty Stack
}

struct stack_node* CreateStackData(List* li){ // Creates new stack node with given list
	// Every stack data is a node of linked list because of implementation
	// every node contains next node and address of a list
	struct stack_node* newNode = (struct stack_node*)malloc(sizeof(struct stack_node)); // Allocates memory from heap
	newNode->next = NULL; // there is no next for newnNode
	newNode->list = li; // sets newNode's data (every node data is an address of list) as given list
	return newNode; // returns new node
}

void push(Stack* stack, List* li){ // Pushes new data to top of stack
	// Because of implementation it means add a to list as head (as first node)
	struct stack_node* newNode = CreateStackData(li); // Creates new Stack node
	if(stack->top != NULL){ // if stack is not empty
		newNode->next = stack->top; // newNode is now points to top (head of list)
		stack->top = newNode; // top is now points to newNode
	}
	else // if stack is empty 
		stack->top = newNode; // top is now points to newNode
}

List* pop(Stack* stack){ // Pops data from stack (returns stack's top value and deletes top Node)
	List* listToRemove = stack->top->list; // Gets list of top
	stack->top = stack->top->next; // top now points top->next, that means we can not reach removedList with stack

	return listToRemove; // returns removed top of stack
}

int isStackEmpty(Stack* stack){ // Chechks is stack empty (is top == NULL), returns 1 if it's empty
	return stack->top == NULL;
}

void PrintStack(Stack* stack){  // Prints stack because of debug purposes, Also it's pops all values
	while(stack->top != NULL){
		PrintList(pop(stack)); // prints list which top of stack
		printf("\n");
	}
}

/* USED ALGORITHM OF SORTING WITH STACK:
1) Create a empty stack say tempStack.
2) While given stack is not empty: 
	- Pop an record from given stack call it temp
	- while new stack is not empty and top of new stack is less than temp:
		pop from new stack and push it to the given stack
	- push temp record in new stack
3) The sorted records are in tempStack
4) Return tempStack
*/

Stack* SortStackbyYear(Stack* stack){  // sorts stack by birth year and returns sorted stack
	Stack* tempStack = CreateStack(); // creates tempStack beacuse of sorting algorithm need 2 stack (given and empty)
	List* temp;
	while(!isStackEmpty(stack)){ // Algorithm pops every data of stack until stack is completely empty
		temp = pop(stack); // pops top and sets temp list as top of stack
		while (!isStackEmpty(tempStack) && (strtoInt(GetBirthYear(tempStack->top->list)) < strtoInt(GetBirthYear(temp)) ) )
		// If stack is not empty and Birth Year of top of tempStack less than Birth Year of temp record
		{ 
			push(stack, pop(tempStack)); // pushes top of temp stack back to main stack
		}
		push(tempStack, temp); // than pushes temp record to top of tempStack
	}	

	return tempStack; // TempStack is currently sorted
}

Stack* SortStackByFacultyCode(Stack* stack){ // sorts stack by Faculty Code and returns sorted stack
// Same as SortStackbyYear except, it uses char* GetID(List*)
	Stack* tempStack = CreateStack();
	List* temp;
	while(!isStackEmpty(stack)){
		temp = pop(stack);
		while (!isStackEmpty(tempStack) && (strtoInt(GetID(tempStack->top->list)) < strtoInt(GetID(temp)) ) )
		{
			push(stack, pop(tempStack));
		}
		push(tempStack, temp);
	}	

	return tempStack;
}

// File Functions
int loadFromFile(Stack* stack, char* fileName){ // loads records from file and fills given stack with this records
	FILE *input; // defines new FILE pointer
    char buf; // char variable buf for getting chars by one by

    if(setlocale(LC_ALL, "Turkish") == NULL){ // Files includes Turkish char's than we need to setlocale as Turkish
        perror("setlocale failed");
        exit(1);
    }

    if ((input = fopen("input.txt","r")) == NULL) // opens file with read mode ("r") and sets input (file pointer)
         return 1; // if input == NULL (file can not be opened beacuse of any reason) return 1
	do{
		List* newList = CreateList(); // Create new list
		do{
			buf = fgetc(input); // get next char from file and assign it to buf
			if(buf != '\n' && buf != EOF) // if this char is not equal to '\n' (newLine) or EOF (end of file)
				AddToEnd(newList, buf); // Add this char to new list
		}while(buf != '\n' && buf != EOF); // repeat until line or file is end

		push(stack, newList); // after create newList, push it to stack
	}while(buf != EOF); // repeat until end of file

    fclose(input); // close the file
}

void PrintToOutFile(char* filename, Stack* stack){ // Prints stack with wanted format
	FILE* fp = fopen(filename, "w"); // opens file with write ("w") mode
	fputs("Student names in ascending order by birthday:\n", fp); // puts this lines to file
 	char *name, *year, *schoolID;
	Stack unsortedStack = *stack; // copy stack as unsortedStack
	stack = SortStackbyYear(stack); // Sorts stack by birth year

	while(!isStackEmpty(stack)){ // while stack is not empty
		List* record = pop(stack); // gets record from stack (a char linked list)
		name = GetName(record); // gets name from record
		year = GetBirthYear(record); // gets birth year from record
		fputs(name,fp); // puts name of record to file
		fputs(";",fp); // puts ';' to file
		fputs(year,fp); // puts birth year of record to file
		fputs("\n",fp); // puts '\n' (new line) to file
	}

	fputs("School numbers by the faculty codes in ascending order:\n",fp);
	stack = SortStackByFacultyCode(&unsortedStack); // sorts stack by faculty code
	while(!isStackEmpty(stack)){ // while stack is not empty
		List* record = pop(stack); // gets record from stack (a char linked list)
		schoolID = GetID(record); // gets schoolID from record
		fputs(schoolID, fp); // puts school ID to record
		fputs("\n",fp); // puts '\n' (new line) to file
	}
}

// Main Function
int main() {
	Stack* stack = CreateStack(); // creates empty stack

	setlocale(LC_ALL, "Turkish"); // sets locale language as Turkish

	loadFromFile(stack, "input.txt"); // load records from "input.txt" and fill stack with records

	PrintToOutFile("output.txt", stack); // Prints records from stack to "output.txt" with wanted format
	return 0; // main returns 0 means everything is fine
}
