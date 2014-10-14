
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 The Parser is a program that reads in the output of the Scanner (HW2) and parses the lexemes (tokens). It must be capable of reading in the tokens produced by your Scanner (HW2) and produce, as output, a message that states whether the PL/0 program is well-formed (syntactically correct) if it follows the grammar rules in Appendix B. Otherwise, if the program does not follow the grammar, a message indicating the type of error present must be printed. A list of the errors to be considered can be found in Appendix C. In addition, the Parser must generate the Symbol Table, which contains all of the variables, procedure names and constants within the PL/0 program. See Appendix E for more information regarding the Symbol Table. If the program is syntactically correct and the Symbol Table is created without error, the execution of the compiler driver continues with intermediate code generation.
 
 *********/
#include <stdio.h>

FILE *originalInputFileWithPCode;
char* originalInputFileLocation = "/Users/jonahkatz/Desktop/hw3_test_case_1/input.txt";

/*
 Input Switches
 */
int shouldPrintStackTrace=0,shouldPrintLexeme=0,shouldPrintAssemblyCode=0;

