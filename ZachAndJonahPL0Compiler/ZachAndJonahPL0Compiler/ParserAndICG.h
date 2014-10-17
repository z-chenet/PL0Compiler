
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 The Parser is a program that reads in the output of the Scanner (HW2) and parses the lexemes (tokens). It must be capable of reading in the tokens produced by your Scanner (HW2) and produce, as output, a message that states whether the PL/0 program is well-formed (syntactically correct) if it follows the grammar rules in Appendix B. Otherwise, if the program does not follow the grammar, a message indicating the type of error present must be printed. A list of the errors to be considered can be found in Appendix C. In addition, the Parser must generate the Symbol Table, which contains all of the variables, procedure names and constants within the PL/0 program. See Appendix E for more information regarding the Symbol Table. If the program is syntactically correct and the Symbol Table is created without error, the execution of the compiler driver continues with intermediate code generation.
 
 The Intermediate Code Generator is a program that takes, as input, the output from the Parser, i.e. the Symbol Table and parsed code. As output, it produces the assembly language for your Virtual Machine (HW1). This functionality will be interleaved with the Parser functionality (i.e. generate assembly code as you parse the token list). Once the code has been generated for your Virtual Machine, the execution of the compiler driver continues by executing the generated assembly code on your Virtual Machine (HW1).

 *********/

#include "Scanner.c"

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct
{
    int kind; 		// const = 1, var = 2, proc = 3
    char name[12];	// name up to 11 chars
    int val; 		// number (ASCII value)
    int level;      // L level
    int addr; 		// M address
} symbol;

typedef struct
{
    int OP;
    int L;
    int M;
    
}instruction;
/*
For constants, you must store kind, name and value.
For variables, you must store kind, name, L and M.
For procedures, you must store kind, name, L and M.
*/

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
Token currentToken;
int currentSymbolTableIndex = 0;
int currentTokenTableIndex = 0;


instruction mCodeTable[MAX_FILE_SIZE];
int currentMCodeTableIndex = 0;

int currentMAddress = 0;
int lexiLevel = 0; //   always zero


void Parser(Token tokenList[MAX_FILE_SIZE], FILE *outputFile);

void getToken();
void block();
void emit(int op, int l, int m);
void constDeclaration();
void parserErrors();
void enterInSymbolTable(int type, Token token, int l, int m, int value);
int convertToInt(char *number);
void varDeclaration();
void procDeclaration();
void statement();
int searchSymbolTableForIdentifier(char *identifier);
void expression();
void term();
void factor();
void finishedProcedure(int level);
void condition();

























