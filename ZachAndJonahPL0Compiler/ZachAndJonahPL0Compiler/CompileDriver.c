
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
    The compiler driver is a program that manages the parts of the compiler. It must handle the input, output, and execution of the Scanner (HW2), the Parser/Code Generator (HW3) and the Virtual Machine (HW1).
 
 *********/

#include "CompileDriver.h"
#include "Scanner.h"

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
    
    
    /********************************************************************************
     
     We have 4 main things to do:
     
     1. Supply the input file to the Scanner which will check for errors and generates the lexeme list to lexeme.txt
     2. Handle any errors (or lack thereof) caught by the Scanner and generate the Symbol table
     3. Pass the symbols to the intermediate code generator which will produce the 'machine code'
     4. Pass the machine code to the Virtual Machine 
     
     
     ********************************************************************************
    
    
    **************************** 1 ****************************/
    
    int doesScannerProduceError = Scanner(originalInputFileWithPCode);
    
    if(doesScannerProduceError) {
        fprintf(<#FILE *restrict#>, <#const char *restrict, ...#>)
    }
    
    /**************************** 2 ****************************/
    
    FILE *lexemeList = fopen(/* this will be the output of the scanner, but for now, its static */"/Users/jonahkatz/Desktop/hw3_test_case_1/lexemelist.txt", "r");
    Scanner(lexemeList);
    
    /**************************** 3 ****************************/
    /**************************** 4 ****************************/

    
    fclose(originalInputFileWithPCode);
    
    
    return 0;
}


