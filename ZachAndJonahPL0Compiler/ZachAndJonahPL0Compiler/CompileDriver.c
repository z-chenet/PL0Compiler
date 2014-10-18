
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
    The compiler driver is a program that manages the parts of the compiler. It must handle the input, output, and execution of the Scanner (HW2), the Parser/Code Generator (HW3) and the Virtual Machine (HW1).
 
 *********/

#include "CompileDriver.h"
//#include "VirtualMachine.c"

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
    
    //int doesScannerProduceError = Scanner(originalInputFileWithPCode, shouldPrintLexeme);
    Scanner(originalInputFileWithPCode);
    
    /* HANDLE PRINTING OF THE LEXEMLIST AND THE ERROR/NO ERORR MESSAGE */
    
    /**************************** 2 AND 3 ****************************/
    
    FILE *mcodeOutput = fopen("/Users/zacharychenet/Google Drive/School/fall14/COP3402SystemSoftware/assignment/ZachAndJonahCodeGenerator/PL0Compiler/ZachAndJonahPL0Compiler/ZachAndJonahPL0Compiler/mcode.txt", "w");
    
    Parser(tokenList);
    printMcodeToFile(mcodeOutput);
    fclose(mcodeOutput);
    
    
    /**************************** 4 ****************************/

    //  virtual machine
    mcodeOutput = fopen("/Users/zacharychenet/Google Drive/School/fall14/COP3402SystemSoftware/assignment/ZachAndJonahCodeGenerator/PL0Compiler/ZachAndJonahPL0Compiler/ZachAndJonahPL0Compiler/mcode.txt", "r" );
    

    
    VirualMachine(mcodeOutput);
    
    //  places print statements here so that if it has to print out anything it it finishes running before printing any of the files out to console
    //  handles prints by printing already made files char by char
    
    if (shouldPrintLexeme == 1) {
        printf("\n");

        printf("\n");
        int c;
        //FILE *file;
        lexemeList = fopen("/Users/zacharychenet/Google Drive/School/fall14/COP3402SystemSoftware/assignment/ZachAndJonahCodeGenerator/PL0Compiler/ZachAndJonahPL0Compiler/ZachAndJonahPL0Compiler/lexemelist.txt", "r");
        if (lexemeList) {
            while ((c = getc(lexemeList)) != EOF)
                putchar(c);
            fclose(lexemeList);
        }
        printf("\n");

    }
    
    if (shouldPrintAssemblyCode == 1) {
        
        printf("\n");
        printf("Assembly Code:\n");
        int c;
        //FILE *file;
        mcodeOutput = fopen("/Users/zacharychenet/Google Drive/School/fall14/COP3402SystemSoftware/assignment/ZachAndJonahCodeGenerator/PL0Compiler/ZachAndJonahPL0Compiler/ZachAndJonahPL0Compiler/mcode.txt", "r");
        if (mcodeOutput) {
            while ((c = getc(mcodeOutput)) != EOF)
                putchar(c);
            fclose(mcodeOutput);
        }
        printf("\n");

    }
    
    //  if the -v argument was made then it goes through and prints everything in the file
    if (shouldPrintStackTrace == 1) {
        printf("\n");

        int c;
        //FILE *file;
        output = fopen("/Users/zacharychenet/Google Drive/School/fall14/COP3402SystemSoftware/assignment/ZachAndJonahCodeGenerator/PL0Compiler/ZachAndJonahPL0Compiler/ZachAndJonahPL0Compiler/stacktrace.txt", "r");
        if (output) {
            while ((c = getc(output)) != EOF)
                putchar(c);
            fclose(output);
        }
        printf("\n");
    }
    
    fclose(originalInputFileWithPCode);
    
    
    return 0;
}


