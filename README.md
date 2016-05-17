# Compiler-and-Optimizer-for-tinyL-

###Intro 
This program interprets a compiler and an optimizer based on lessons and assignments currently in the subject. The program works with a doubly linked list of instructions. A compiler is writened to make some instructions in which the optimizer will take a look at the ouput file and determine which code is redundent.

###Compiler 
The compiler will generate RISC machine instructions called ILOC (Intermediate Language for Optimizing Compilers)


###Optimizer
The optimizer that takes ILOC instructions as input and implements dead code elimination.The output of the optimzer is a sequence of ILOC instructions which produces the same results as the original input sequence. deallocating "dead" instructions is necessary to avid memory leaks.
