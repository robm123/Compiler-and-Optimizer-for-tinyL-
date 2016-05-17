/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2015                                *
 *  Roberto Merino                           *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"
 
void setArrays(char offset[], char registers[], int end); 

int compareRegisters(char registers[], int num);
int compareOffset(char offset[], int num);
                  
int main()
{
	Instruction *head;
	Instruction *current;
	Instruction *traverse;
	Instruction *temp;
	
	int end = 30;
	char offset[end];
	char registers[end];
	setArrays(offset, registers, end)		 //initializes arrays needed;
	
	head = ReadInstructionList(stdin);
	if (!head) 
	{
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
	current = head;
	
	while(current->opcode != OUTPUTAI)                               
	{
		current->critical = 'c';                      //char critical 'c' == crucial ........'n' == not crucial instruction
		current = current->next;                      //LOADI, LOADAI, STOREAI, OUTPUTAI
	}
	
	current->critical = 'c';
	
	int rcount = 0;
	int ocount = 0;
	
	int badOffset = -1; 
	
	int b;									  
	
	b = current->field2;				
	current = current->prev;		
	int delete = 0;														//LOADI, LOADAI, STOREAI, ADD, SUB, MUL, DIV, OUTPUTAI

	while(current->prev != NULL)         
	{	
		switch(current->opcode)
		{	
			case STOREAI:
					if(current->field3 != b && (current->next)->opcode == OUTPUTAI)//ONLY RUNS FOR TEST 4 AND 5 
					{	
						current->critical = 'n';      	//marked for deletion
						badOffset = 1;		//flags bag offset  1 = BAD  2 = GOOD
					}
					else if(compareOffset(offset, current->field3) == 1)	//checks offsets
					{	
						badOffset = 2;
						resetRegisters(registers);
						offset[delete]= -1;
					    	delete++;
						rcount = 0;		// resets counters
						registers[rcount] = current->field1;
						current->critical = 'c';
					}
					else if(current->field3 == b)	//takes care of inital case.. ouputai->storeai
					{		

						setArrays(offset, registers, end);		//RESET ARRAYS
						rcount = 0;					// resets counters
						ocount = 0;					// resets counters
						badOffset = 2;					
						current->critical = 'c'; 
						registers[rcount] = current->field1; 	
						rcount++;
						b = 99;
					}
					else if(compareOffset(offset, current->field3) != 1)//NO OFFSET FOUND
					{	
						//printf("OFFSETS DONT MATCH r4 -> %d", current->field1);
						badOffset = 1;			//BAD
						resetRegisters(registers);
						rcount = 0;
						current->critical = 'n';   	//marking for deletion
						registers[rcount] = current->field1;
						rcount++;
					}

					break;
			case MUL: 
			case ADD:
			case SUB:
			case DIV:
					if(badOffset == 1)//BAD
					{	
					     registers[rcount] = current->field1;
					     rcount++;
					     registers[rcount] = current->field2;
					     rcount++; 
					     current->critical = 'n'; 
						
					}
					if(badOffset == 2)//GOOD
					{	
						//save conditions for keeping
						if(compareRegisters(registers, current->field3) ==  1)
						{	
							registers[rcount] = current->field1;
							rcount++;
							registers[rcount] = current->field2;
							rcount++;
							current->critical = 'c';
						}
					}
					break;
			case LOADI:
					if(badOffset == 1) //BAD
					{
						current->critical = 'n';   
					}
					else if(badOffset == 2)//GOOD
					{	
						if(compareRegisters(registers, current->field2) == 1)
						{		
							current->critical = 'c';
						}
					}
					else if(current->field1 == 1024)  //NEVER REACHES FIRST STATEMENT
					{	
						printf("\n*****LAST****\n");
						current->critical = 'c';
					}
					break;
			case LOADAI:

					if(badOffset == 2)
					{	
						if(compareRegisters(registers, current->field3) == 1)
						{	
							offset[ocount] = current->field2;
							ocount++;
							current->critical = 'c';
						}
					}
					else if(badOffset == 1)//BAD
					{
						if(compareRegisters(registers, current->field3) == 1)
						{	
							current->critical = 'n';
						}	
					}
					break;
			default:
				printf("ERROR");
		}
		current = current->prev;
	}
	
	traverse = head;

	while(traverse != NULL)
	{
		if(traverse->critical == 'n')
		{	

			temp = traverse;
			traverse->next->prev = traverse->prev;
			traverse->prev->next = traverse->next;
			free(temp);
		}

		traverse = traverse->next;
	}
	
	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

void setArrays(char offset[], char registers[], int end)
{
	int counter;
	for(counter = 0; counter< end;counter++)
	{
		offset[counter] = -1;
		registers[counter] = -1;
	}
	
}

void resetRegisters(char registers[])
{
	int counter1;
	int size1 = sizeof(registers)/sizeof(registers[0]);
	
	for(counter1 = 0; counter1< size1;counter1++)
		registers[counter1] = -1;	
}


int compareRegisters(char registers[], int num)		//COMPARES REGISTERs
{
	int end = 30;
	int x;  
	for(x = 0; x < end; x++)
	{	
		if(registers[x] == num) // if finds, returns FOUND
		{		
			return 1;
		}
	}
	return 0; //default value NOT FOUND
}

int compareOffset(char offset[], int num)		//COMPARESOFFSETS
{
	int size = sizeof(offset)/sizeof(offset[0]);
	int x;
	
	for(x = 0; x < 30;x++)
	{
		if(offset[x] == num) // if finds, returns FOUND
			return 1;
	}
	
	return 0; //default value NOT FOUND
}

void resetOffsets(char offset[])
{
	int x;
	int size = sizeof(offset)/sizeof(offset[0]);
	
	for(x = 0; x < 30;x++)
	{
			offset[x] = -1;
	}
}
