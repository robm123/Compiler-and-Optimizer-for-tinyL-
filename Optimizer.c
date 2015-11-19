/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2015                                *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
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
	setArrays(offset, registers, end); //initializes arrays needed;
	
	head = ReadInstructionList(stdin);
	if (!head) {
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
	
	//we are now at OUTPUTAI now must save the offset field 2
	int rcount = 0;
	int ocount = 0;
	
	int badOffset = -1; // 1 bad ....2 good?
	
	int b;									  
	
	b = current->field2;							  	  //offset must match previous nodes offset INITAL OFFSET
	current = current->prev;							  //since saved offset go back one node to STOREAI
	int delete = 0;														//LOADI, LOADAI, STOREAI, ADD, SUB, MUL, DIV, OUTPUTAI
	//printf("\n1 before the while stmt\n");
	while(current->prev != NULL)  //NOW AT STOREAI         
	{	
		
		switch(current->opcode)
		{	
			case STOREAI:
					if(current->field3 != b && (current->next)->opcode == OUTPUTAI)//ONLY RUNS FOR TEST 4 AND 5 
					{	
						//printf("\nRUNS ONLY ONCE\n");	
						current->critical = 'n';      	//marked for deletion
						//registers[rcount] = field1;   //save field1 dont care as it is not needed
						//count++;	
						badOffset = 1;					//flags bag offset  1 = BAD  2 = GOOD			
						//break;		
					}
					else if(compareOffset(offset, current->field3) == 1 /*|| badOffset==2*/)//checks offsets
					{	
						//printf("OFFSETS MATCH! %d \n", ocount);
						badOffset = 2; // GOOD  STILLL GOOD 
						resetRegisters(registers);
						//reset off sets???
					    //resetOffsets(offset);
					    // ocount = 0;
					    //printf("OFFSET %d\n", offset[ocount-1]);
					    offset[delete]= -1;
					    delete++;
						rcount = 0;								// resets counters
						registers[rcount] = current->field1;
						current->critical = 'c';
					}
					else if(current->field3 == b)				//offsets are the same 		//takes care of inital case.. ouputai->storeai
					{		
						//	printf("\nDo the offsets match ? : %d.......%d\n", compareOffset(offset,current->field3), current->field3);
							setArrays(offset, registers, end);		//RESET ARRAYS
							rcount = 0;								// resets counters
							ocount = 0;								// resets counters
							badOffset = 2;							//starting at good OFFSET 
							current->critical = 'c'; 
							registers[rcount] = current->field1; 	//SINCE GOOD OFFSET FIELD1 IMPORTANT
							rcount++;
							b = 99;
							
					}
					else if(compareOffset(offset, current->field3) != 1)//NO OFFSET FOUND
					{	
						//printf("OFFSETS DONT MATCH r4 -> %d", current->field1);
						badOffset = 1;		//BAD
						resetRegisters(registers);
						//resetOffsets(offset);
						//ocount = 0;
						rcount = 0;
						current->critical = 'n';   //marking for deletion
						registers[rcount] = current->field1;
						rcount++;
					}
					//checks for good
					
					//check field 3 of 
					
					//possible else stmt
					//RESET ARRAYS AND START ALL OVER AGAIN
					break;
			case MUL: 
			case ADD:
			case SUB:
			case DIV:
				//	printf("\n\n");
					if(badOffset == 1)//BAD
					{	
						//may not need arrays for badoffsets as boolean takes care of it 
					     //save conditions for deletion
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
							//printf("****R3 AND R4 appear*** r%d   r%d\n", current->field1, current->field2);
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
						// if(compare(registers, current->field2) == 1)
						 //{
							 //current->critical = 'n';
						// }
						current->critical = 'n';   //may only need this line as we know badOffset is true so marking for deletiong unitl reach STORAI 
						//printf("***helP-");
					//just compare field2

					//just comparing
					//possible function that returns o or 1 if found;
					}
					else if(badOffset == 2)//GOOD
					{	
						if(compareRegisters(registers, current->field2) == 1)
						{		
								//printf("fields match-");
								current->critical = 'c';
						}
								
						//printf("LOADI 3 r%d\n",current->field2); 
							
					}
					else if(current->field1 == 1024)  //NEVER REACHES FIRST STATEMENT
					{	
						printf("\n*****LAST****\n");
						//DEFAULT BEGINING VALUE    loadI 1024 => r0
						//if(current->prev == NULL)
						//{
							current->critical = 'c';
							
						//}
					}
					break;
			case LOADAI:
					//printf("INSIDE LOADAI!!\n ");
					//2 conditions 1 for test 3 ......1 for the rest 
					//first match field3 then save offset
					if(badOffset == 2)//??????GOOOD
					{	
						//printf("\nGOOD!!!****field 3 -> %d ==== %d\n", current->field3, registers[8]);
						
						//printf("\n%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",registers[0],registers[1],registers[2],registers[3],registers[4],registers[5],registers[6],registers[7],registers[8],registers[9]);
						
						//int help = compareRegisters(registers, 4);
						//printf("----->%d\n", help);
						
						if(compareRegisters(registers, current->field3) == 1)
						{	
							
							//save offset to array for later reference
							offset[ocount] = current->field2;
							//printf("*****-> %d\n", offset[ocount]);
							ocount++;
							current->critical = 'c';
						}
						//possible else
						////////////
					}
					else if(badOffset == 1)//BAD
					{
						//printf("SHOULD NOT PRINT");
						if(compareRegisters(registers, current->field3) == 1)
						{	
							//save offset to array for later reference
							//offset[ocount] = current->field2;
							//ocount++;
							current->critical = 'n';
						}	
					}
					break;
			default:
				printf("ERROR");
		}
		//printf("\n->> %d ", current->field1);
		current = current->prev;	//traversing backwards to the top stops at the begining node LOADI
	}
				
	
	
	//printf("-----------------------------");
	
	//current = current->prev;
/*	
	while(current->opcode != OUTPUTAI)
	{
		if(current->critical == 'n');
		{
			if(current->opcode == LOADI)
				printf("\nLOADI : num -> %d\n", current->field1);
			if(current->opcode == LOADAI)
				printf("\nLOADAI: num -> %d\n", current->field1);
			if(current->opcode == STOREAI)
				printf("\nSTOREAI: num -> %d\n", current->field1);
			if(current->opcode == ADD)
				printf("\nADD: num -> %d\n", current->field1);
			if(current->opcode == SUB)
				printf("\nSUB: num -> %d\n", current->field1);
			if(current->opcode == DIV)
				printf("\nDIV: num-> %d\n",current->field1);
			if(current->opcode == MUL)
				printf("\nMUL: num -> %d\n", current->field1);
			if(current->opcode == OUTPUTAI)
				printf("\nOUTPUTAI: num -> %d\n", current->field1);	
		}
	}
*/	
	
	
	
	/* YOUR CODE GOES HERE */

	/////////////////////////////////////////////////////////LOADI, LOADAI, STOREAI, ADD, SUB, MUL, DIV, OUTPUTAI
	
	traverse = head;
	
	//char x = 'x';
	/*
	while(traverse != NULL)                               
	{
		
		//printf("%c\n", current->critical);
	
		//if(traverse->critical == 'c')
		{
			//printf("only a few times\n");
			
			if(traverse->opcode == LOADI)
				printf("\nLOADI : num -> %d\t critical: %c", traverse->field1, traverse->critical);
			if(traverse->opcode == LOADAI)
				printf("\nLOADAI: num -> r%d\t critical: %c", traverse->field1,traverse->critical);
			if(traverse->opcode == STOREAI)
				printf("\nSTOREAI: num -> r%d\t critical: %c", traverse->field1,traverse->critical);
			if(traverse->opcode == ADD)
				printf("\nADD: num -> r%d\t\tcritical: %c", traverse->field1,traverse->critical);
			if(traverse->opcode == SUB)
				printf("\nSUB: num -> r%d\t critical: %c", traverse->field1,traverse->critical);
			if(traverse->opcode == DIV)
				printf("\nDIV: num-> r%d\t critical: %c",traverse->field1,traverse->critical);
			if(traverse->opcode == MUL)
				printf("\nMUL: num -> r%d\t\tcritical: %c", traverse->field1,traverse->critical);
			if(traverse->opcode == OUTPUTAI)
				printf("\nOUTPUTAI: num -> r%d\t critical: %c\n", traverse->field1,traverse->critical);
				
		}//
		
		traverse = traverse->next;                     
	}
	*/

//delete nodes


	while(traverse != NULL)
	{
		if(traverse->critical == 'n')
		{	
			//if( traverse->next != NULL)
			//{
				temp = traverse;
				traverse->next->prev = traverse->prev;
				//free(temp);
			//}
			//else if(traverse->prev != NULL)
			//{
				//temp = traverse;
				traverse->prev->next = traverse->next;
				//free(temp);
			//}
			free(temp);
		}

		traverse = traverse->next;
	}
/*	**********************THIS PRINTS*************************
	Instruction *YASS;
	YASS = head;
	
	while(YASS != NULL)                               
	{
		if(YASS->opcode == LOADI)
			printf("\nLOADI : num -> %d\t critical: %c", YASS->field1, YASS->critical);
		if(YASS->opcode == LOADAI)
			printf("\nLOADAI: num -> r%d\t critical: %c", YASS->field1,YASS->critical);
		if(YASS->opcode == STOREAI)
			printf("\nSTOREAI: num -> r%d\t critical: %c", YASS->field1,YASS->critical);
		if(YASS->opcode == ADD)
			printf("\nADD: num -> r%d\t\tcritical: %c", YASS->field1,YASS->critical);
		if(YASS->opcode == SUB)
			printf("\nSUB: num -> r%d\t critical: %c", YASS->field1,YASS->critical);
		if(YASS->opcode == DIV)
			printf("\nDIV: num-> r%d\t critical: %c",YASS->field1,YASS->critical);
		if(YASS->opcode == MUL)
			printf("\nMUL: num -> r%d\t\tcritical: %c", YASS->field1,YASS->critical);
		if(YASS->opcode == OUTPUTAI)
			printf("\nOUTPUTAI: num -> r%d\t critical: %c\n", YASS->field1,YASS->critical);
					
		YASS = YASS->next;                     
	}

*///**************************************


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


int compareRegisters(char registers[], int num)		//COMPARES REGISTERS!!!!
{
	
	//int size = sizeof(registers)/sizeof(registers[0]);
	int end = 30;
	int x;  
	for(x = 0; x < end; x++)
	{	
	  // printf("registers[%d] : %d\n", x,registers[x]);
		
		if(registers[x] == num) // if finds, returns FOUND
		{		
		//		printf("found%d", num);
				return 1;
		}
			
	}

	return 0; //default value NOT FOUND
}

int compareOffset(char offset[], int num)		//COMPARESOFFSETS!!!!
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
