
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 *********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//  constants for file
#define MAX_FILE_SIZE 1000
#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5
#define RESERVED_WORDS 14
#define SPECIAL_SYMBOLS 13
#define INVICIBLE_WORDS 4

//  different numbers repersenting the different set token types
typedef enum
{
    nulsym = 1,
    identsym,
    numbersym,
    plussym,
    minussym,
    multsym,
    slashsym,
    oddsym,
    eqsym,
    neqsym,
    lessym,
    leqsym,
    gtrsym,
    geqsym,
    lparentsym,
    rparentsym,
    commasym,
    semicolonsym,
    periodsym,
    becomessym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    writesym,
    readsym,
    elsesym
    
}tokenType;

//  array with the string for the corresponding tokenType
char *tokenTypeWords[] = {
    " ",
    "nulsym",
    "identsym",
    "numbersym",
    "plussym",
    "minussym",
    "multsym",
    "slashsym",
    "oddsym",
    "eqsym",
    "neqsym",
    "lessym",
    "leqsym",
    "gtrsym",
    "geqsym",
    "lparentsym",
    "rparentsym",
    "commasym",
    "semicolonsym",
    "periodsym",
    "becomessym",
    "beginsym",
    "endsym",
    "ifsym",
    "thensym",
    "whilesym",
    "dosym",
    "callsym",
    "constsym",
    "varsym",
    "procsym",
    "writesym",
    "readsym",
    "elsesym",
};

//  array of the strings corresponding to the different reserved words
char *reservedWords[] = {
    "const",
    "var",
    "procedure",
    "call",
    "begin",
    "end",
    "if",
    "then",
    "else",
    "while",
    "do",
    "read",
    "write",
    "odd"
};

//  int array corresponding to the reserved words
int reservedWordsInternal[] = {
    constsym,
    varsym,
    procsym,
    callsym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    elsesym,
    whilesym,
    dosym,
    readsym,
    writesym,
    oddsym
};

//  char array representing the special symbol chars
char specialSymbols[] = {
    '+',
    '-',
    '*',
    '/',
    '(',
    ')',
    '=',
    ',',
    '.',
    '<',
    '>',
    ';',
    ':'
};

//  int array corresponding to the reserved words
int specialSymbolsInternal[] = {
    plussym,
    minussym,
    multsym,
    slashsym,
    lparentsym,
    rparentsym,
    eqsym,
    commasym,
    periodsym,
    lessym,
    gtrsym,
    semicolonsym,
    becomessym  //  when read colon it just becomes
};

//  char array for the invisible char
char invisibleCharacters[] = {
    ' ',
    '\n',
    '\t',
    '\r'
};

//  struct for Tokens
typedef struct{
    tokenType tokenID;
    char name[IDENTIFIER_MAX_LENGTH];
    int index;
} Token;

//  creats tokenList to hold all tokens
Token tokenList[MAX_FILE_SIZE];

//  variable to keep track of tokens in tokenList
int tokenIndex = 0;

//  Files pointers for the input and output
FILE *input;
//FILE *cleanInput;
FILE *lexemeList;
//FILE *lexemeTable;

/*
int isReservedWord(char *word);
int isSpecialChar(char currentChar);
int isInvisibleChar(char currentChar);
void printInput(char currentChar);
void printLexemeTable();
void printLexemeList();
void error(int code);*/


/*******
 
 * The inputFile contains a P machine program
 * Scans and produces lexeme table.
 * Returns 1 if there is an error, 0 otherwise
 
 *******/
int Scanner(FILE* inputFile);

//  function to check if the current word is a possible reserved word
int isReservedWord(char *word){
    int i;
    for (i = 0; i < RESERVED_WORDS; i++) {
        if (strcmp(word, reservedWords[i]) == 0) {
            return reservedWordsInternal[i];
        }
    }
    
    return -1;
}

//  function to check if the current char is a possible special symbol
int isSpecialChar(char currentChar){
    int i;
    for (i = 0; i < SPECIAL_SYMBOLS; i++) {
        if (currentChar == specialSymbols[i]) {
            return specialSymbolsInternal[i];
        }
    }
    return -1;
}

//  add print invisible char to clean input
int isInvisibleChar(char currentChar){
    int i;
    for (i = 0; i < INVICIBLE_WORDS; i++) {
        if (currentChar == invisibleCharacters[i]) {
            return 1;
        }
    }
    
    return 0;
}

//  function to handle errors
void error(int code){
    //  switch statement to handle different errors
    switch (code) {
        case 1:
            printf("Error occured:: Variable does not start with letter\n\n");
            break;
        
        case 2:
            printf("Error occured:: Number is too long\n\n");
            break;
            
        case 3:
            printf("Error occured:: Name too long\n\n");
            break;
        
        case 4:
            printf("Error occured:: Invalid Symbols\n\n");
            break;
            
        default:
            break;
    }
    
    //  makes the program end after an error has occured instead of finishing the code
    exit(1);
}

/*
//  function to handle printing the clean input
void printInput(char currentChar){
    fprintf(cleanInput, "%c", currentChar);
}

//  function to handle printing the lexemetable
void printLexemeTable(){
    fprintf(lexemeTable, "Lexeme Table:\n");
    fprintf(lexemeTable, "Lexeme\t\tToken Type\n");
    
    //  for loop to print lexemetable in columns with the name of the token followed by the id number
    int i;
    for (i = 0; i < tokenIndex; i++) {
        fprintf(lexemeTable, "%-11s\t%d\n", tokenList[i].name, tokenList[i].tokenID);
    }
    fprintf(lexemeTable, "\n");
}*/

//  function to handle printing the lexemelist
void printLexemeList(){
    fprintf(lexemeList, "Lexeme List:\n\n");
    //  for loop to print the lexemelist in order and adds the values if of the possible variables if necessary
    int i;
    for (i = 0; i < tokenIndex; i++) {
        if (tokenList[i].tokenID == 2 || tokenList[i].tokenID == 3) {
            fprintf(lexemeList, "%d %s ", tokenList[i].tokenID, tokenList[i].name);
        }
        else{
            fprintf(lexemeList, "%d ", tokenList[i].tokenID);
        }
    }
    fprintf(lexemeList, "\n");
}











