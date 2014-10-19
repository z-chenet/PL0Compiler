
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
    The compiler driver is a program that manages the parts of the compiler. It must handle the input, output, and execution of the Scanner (HW2), the Parser/Code Generator (HW3) and the Virtual Machine (HW1).
 
 *********/

#include "VirtualMachine.c"

FILE *originalInputFileWithPCode;
//char* originalInputFileLocation = "input.txt";
char* originalInputFileLocation = "input.txt";

/*
 Input Switches
 */
int shouldPrintStackTrace=0,shouldPrintLexeme=0,shouldPrintAssemblyCode=0;

