
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 The compiler driver is a program that manages the parts of the compiler. It must handle the input, output, and execution of the Scanner (HW2), the Parser/Code Generator (HW3) and the Virtual Machine (HW1).
 
 *********/

#include "CompileDriver.h"
int main(int argc, const char * argv[])
{


    /*
     Check input switches
    */
    int i=0;
    for (i=0; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            shouldPrintStackTrace = 1;
        }
        else if ( strcmp(argv[i], "-l") == 0)
        {
            shouldPrintLexeme = 1;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            shouldPrintAssemblyCode = 1;
        }
    }
    
    originalInputFileWithPCode = fopen(originalInputFileLocation, "r");

    
    
    
    return 0;
}

