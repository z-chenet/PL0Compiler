
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
    The compiler driver is a program that manages the parts of the compiler. It must handle the input, output, and execution of the Scanner (HW2), the Parser/Code Generator (HW3) and the Virtual Machine (HW1).
 
 *********/

#include <stdio.h>

FILE *originalInputFileWithPCode;
char* originalInputFileLocation = "/Users/jonahkatz/Desktop/hw3_test_case_1/input.txt";

/*
 Input Switches
 */
int shouldPrintStackTrace=0,shouldPrintLexeme=0,shouldPrintAssemblyCode=0;

