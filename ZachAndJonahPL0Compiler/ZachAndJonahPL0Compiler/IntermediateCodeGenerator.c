
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 The Intermediate Code Generator is a program that takes, as input, the output from the Parser, i.e. the Symbol Table and parsed code. As output, it produces the assembly language for your Virtual Machine (HW1). This functionality will be interleaved with the Parser functionality (i.e. generate assembly code as you parse the token list). Once the code has been generated for your Virtual Machine, the execution of the compiler driver continues by executing the generated assembly code on your Virtual Machine (HW1).
 
 *********/



#include "IntermediateCodeGenerator.h"
