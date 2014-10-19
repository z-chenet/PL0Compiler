
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
    
    //  opens input.txt
    originalInputFileWithPCode = fopen(originalInputFileLocation, "r");

    //  scans in the input and saves it to lexemelist inside the Scanner file
    Scanner(originalInputFileWithPCode);

    
    //  opens and handles the parser call
    //  closes file after so we can open to be read later for the virtual machine
    FILE *mcodeOutput = fopen("mcode.txt", "w");
    
    Parser(tokenList);
    printMcodeToFile(mcodeOutput);
    fclose(mcodeOutput);
    
    //  virtual machine
    mcodeOutput = fopen("mcode.txt", "r" );
    

    
    VirualMachine(mcodeOutput,shouldPrintStackTrace);
    
    //  places print statements here so that if it has to print out anything it it finishes running before printing any of the files out to console
    //  handles prints by printing already made files char by char
    
    if (shouldPrintLexeme == 1) {
        int c;
        //FILE *file;
        lexemeList = fopen("lexemelist.txt", "r");
        if (lexemeList) {
            while ((c = getc(lexemeList)) != EOF)
                putchar(c);
            fclose(lexemeList);
        }
        printf("\n");
        
            if (errorOccured == 0) {
                printf("No errors, program is syntactically correct.\n");
            }
    }
    
    if (shouldPrintAssemblyCode == 1) {
        
        printf("\n");
        printf("Assembly Code:\n");
        int c;
        //FILE *file;
        mcodeOutput = fopen("mcode.txt", "r");
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
        FILE *file;
        file = fopen("stacktraceTemp.txt", "r");
        if (file) {
            while ((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
        }
        printf("\n");
        
    }
    
    fclose(originalInputFileWithPCode);
    
    
    return 0;
}


