#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Struct define
struct node{
	int data;
	struct node *nextcol;
	struct node *nextrow;
};

typedef struct{
	struct node *head;
}matrix;


// Function blueprints
matrix *createMatrix(); // creates empty matrix with head = NULL and returns new matrix
struct node *createNode(int); // creates empty node with given data and returns new node
void printMatrix(matrix*); // prints matrix to cmd, debug only
void insertNode(matrix*, int, int, int); // inserts given int data to matrix in given row and column 
int loadFromCsv(matrix*, char*); // loads csv data to given matrix
int writeMatrixFlat(matrix*, char*); // writes the given matrix as csv to given filename
int writeMatrixSpecial(matrix*, char*); // writes the matrix, diagonal and top to bottom

// Functions
matrix *createMatrix(){
	matrix *newMatrix = (matrix *)malloc(sizeof(matrix));
	// We need a new matrix in heap, malloc function allocates an empty memory storage
	// (matrix *) part indicates the memory storage data type 
	
	newMatrix->head = NULL;
	// We defines newMatrix's head as NULL
	// That means matrix is currently empty
	
	return newMatrix;
	// returns the address of matrix
	// if the matrix wasn't in the heap, we wouldn't be able to return
	// because, function stacks are cleared after return
}

struct node *createNode(int data){
	struct node *newNode = (struct node *)malloc(sizeof(struct node));
	// Same as new matrix, only we used the (struct node*) type and size of one node
	// Node includes an data (int) and two node pointers
	
	newNode->data = data;
	// Sets newNode's data as given data parameter
	newNode->nextcol = NULL;
	newNode->nextrow = NULL;
	// nextcol and nextrow is currently empty
	
	return newNode;
	// returns node address
}
void insertNode(matrix *m, int data, int row, int col){
	struct node *upNode = NULL; // upNode refers to one position up to the target pos
	struct node *leftNode = NULL; // leftNode refers to one position left to the target pos
	struct node *newNode = createNode(data); // newnode is the node we will add, we call createNode func for creating new node
	
	int i;
	if(row == 1 && col == 1){ // if position is (1,1) that means newNode is first node of matrix
		m->head	= newNode; // as a result, head is directly refers to our new node 
		return;
	}
	
	if(row>1){ // if row is not greater than 1 (that means row = 1), there is no upNode 
		upNode = m->head; // upNode pos starts from head
		
		
		for(i=1; i<row-1; i++){
			upNode = upNode->nextrow;
		} // sets upNode row to one up from the target pos
		
		for(i=0; i<col-1; i++){
			upNode = upNode->nextcol;
		} // sets upNode column equal to target pos
		
		upNode->nextrow = newNode; // Than, upNode->nextrow (1 position below) is refers to newNode
	}
	
	if(col>1){ // if col is not greater than 1 (that means col = 1), there is no leftNode 
		leftNode = m->head; // leftNode pos starts from head
		
		for(i=1; i<col-1; i++){
			leftNode = leftNode->nextcol;
		} // sets leftNode col to one left from the target pos
		
		for(i=0; i<row-1; i++){
			leftNode = leftNode->nextrow;
		} // sets leftNode row equal to target pos
		
		leftNode->nextcol = newNode; // Than, leftNode->nextcol (1 position right) is refers to newNode
	}
		
	
}

void printMatrix(matrix* m){ // Prints matrix to CMD directly, onşy for debug

	struct node *temprow;
	struct node *tempcol;
	
	temprow = m->head;
	tempcol = m->head;
	while(temprow != NULL){
		while(tempcol != NULL){
			printf(" %d ", tempcol->data);
			tempcol = tempcol->nextcol;
		}
		temprow = temprow->nextrow;
		tempcol = temprow;
		printf("\n");
	}
	// loops step by step and prints the data
}

int writeMatrixFlat(matrix *m, char* fileName) // Writes matrix to txt file
{
	FILE* fp = fopen(fileName, "w"); // We will write data, therefore we need to open file in Write mode ("w")
	// FILE* referst to file pointer 
	
	if(!fp) // If we can't open
		return 1; // return 1, that means there is an error

	struct node *currentRow; // holds to first node of current row 
	struct node *temp; // hold current node
	char strBuffer[30]; // converting int to string buffer
	
	currentRow =  m->head; // currentrow is first row
	temp = currentRow; // current node is currentRow
	
	
	while(currentRow != NULL){ // if there is still row to write
		while(temp != NULL){ // if there is still col to write
			itoa(temp->data, strBuffer, 10); // converts decimal (last parameter refers) data to string and save it to in a buffer
			fputs(strBuffer, fp); // Writes buffer data to fp (our file pointer), than moves the pointers pos
			if(temp->nextcol != NULL) // if current node is not last column of row
				fputs(",", fp); // put comma

			temp = temp->nextcol; 
		}
		
		if(currentRow->nextrow != NULL) // if current row is not last row of matrix
			fputs("\n", fp); // write \n (new line)
		
		currentRow = currentRow->nextrow; // current row is next row
		temp = currentRow; // current node is first node of current row
	}
	
	fclose(fp);
}

int writeMatrixSpecial(matrix *m, char *fileName){ // writes diagonal and top to bottom
	
	FILE *fp = fopen(fileName, "w");
	
	if(!fp)
		return 1;
		
	struct node* temp;
	struct node* currentCol;
	char strBuffer[30];
	temp = m->head;
	
	// The upper part is the same as writeMatrixFlat 
	
	while(1){ // This infinite loop continues until calling the "break" command
		itoa(temp->data, strBuffer, 10); // convert current node data to string
		fputs(strBuffer, fp); // writes data to fp
		
		if(temp->nextcol != NULL && temp->nextrow != NULL){ // if current node is not last node
			temp = temp->nextrow->nextcol; // set current node as next node (next diagonal node)
			fputs(",", fp);	// write comma
		}
			
		else // if it is the last node
			break; // break the loop

	}	
	
	fputs("\n", fp); // write new line
	
	temp = m->head; // set current node as head
	currentCol = temp; // set current column node as head
	 
	while(currentCol != NULL){ // if there is still column to write
		while(temp != NULL){ // if there is still row to write
			itoa(temp->data, strBuffer, 10); // converts decimal node data to string
			fputs(strBuffer, fp); // writes string data to file
			
			if(temp->nextrow != NULL) // if current node is not last row
				fputs(",", fp); // write comma
				
			temp = temp->nextrow; 
		}
		currentCol = currentCol->nextcol;
		temp = currentCol;
		
		if(temp != NULL)
			fputs(",", fp);
	}
	
	fclose(fp);
}

int loadFromCsv(matrix *m, char* fileName){  // loads matrix data from given CSV file
	
	FILE *fp = fopen(fileName, "r"); // Opens file in read mode
	
	if(!fp)
		return 1;
	else
	{
		char buffer[1024]; // A buffer for reading chars from file
		
		int row    = 0; // current row index
		int column = 0; // current column index
		
		while(fgets(buffer, 1024, fp)){ // fgets() try to gets 1024 byte from fp and stores in the buffer
		// fgets returns 0 if there is no more byte to read
		 
			column=0;
			row++; // set to next row index
			
			char* value = strtok(buffer, ","); // it splits char array from ','
			while (value) {
				column++;
				insertNode(m, atoi(value), row, column); // inserts value to matrix
				// atoi functions converts char array to integer
					
                value = strtok(NULL, ",");
	    	}
		}	
	}
	
	fclose(fp);
	return 0;
}

// Main
int main(int argc, char *argv[]) {
	matrix *m = createMatrix(); // m is our matrix
	// createMatrix returns empty matrix
	
	loadFromCsv(m, "input.csv"); 
	// loadFromCsv fills m matrix with file data

	writeMatrixFlat(m, "output_print.txt");
	// writes m matrix to output_print.txt
	
	writeMatrixSpecial(m, "output.txt");
	// writes m matrix to output.txt as diagonal and top to bottom
}
