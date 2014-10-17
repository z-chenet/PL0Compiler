
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 *********/

#include "Scanner.h"


/*******
 
 * The inputFile contains a P machine program
 * Scans and produces lexeme table.
 * Returns 1 if there is an error, 0 otherwise
 
 *******/

int Scanner(FILE* inputFile)
{
    
    //  initial variables
    char currentIndexCharacter;
    int specialSymbolChar;
    
    input = inputFile;
    //inputFile = fopen(<#const char *restrict#>, <#const char *restrict#>)
    lexemeList = fopen("lexemelist.txt", "w");

    
    
    //  clean input still messing up with periods commas and special symbols
    while ((currentIndexCharacter = getc(input)) != EOF) {
        Token token;

        //  variable used to keep track of identifier index
        int i = 0;
        
        //  check to see if the current character is a letter
        if (isalpha(currentIndexCharacter) != 0) {
            //  prints to cleaninput file
            //printInput(currentIndexCharacter);
            
            //  creates and places the current letter into possible word
            char identifier[IDENTIFIER_MAX_LENGTH];
            identifier[i] = currentIndexCharacter;
            i++;
            
            currentIndexCharacter = fgetc(input);
            //  prints current symbol to the clean input
            //  prints current symbol to cleaninput
            //  checks if the next symbol after is a period or comma
            if (currentIndexCharacter != ',' && currentIndexCharacter != ';' && currentIndexCharacter != '.'){
                //printInput(currentIndexCharacter);
            }
            
            //  continue if the next character is a number or letter
            while (isalnum(currentIndexCharacter)) {
                identifier[i] = currentIndexCharacter;
                i++;
                currentIndexCharacter = fgetc(input);
                
                //  prints current symbol to cleaninput
                //  checks if the next symbol after is a period or comma 
                if (currentIndexCharacter != ',' && currentIndexCharacter != ';' && currentIndexCharacter != '.'){
                        //printInput(currentIndexCharacter);
                }
                
            }
            
            //  check to see if identifier is to long and report if so
            if (i > IDENTIFIER_MAX_LENGTH) {
                error(3);
            }
            else{
                //  puts current char back in input file
                ungetc(currentIndexCharacter, input);
                
                //  sets end of word to the terminating string
                identifier[i] = '\0';
                
                //  checks if word is a reserved word
                //  handles accordingly with the if statement
                int wordID = isReservedWord(identifier);
                
                if (wordID >= 0) {
                    token.tokenID = wordID;
                    strcpy(token.name, identifier);
                    token.index = tokenIndex;
                    tokenList[tokenIndex] = token;
                    tokenIndex++;
                }
                else{
                    token.tokenID = identsym;
                    strcpy(token.name, identifier);
                    token.index = tokenIndex;
                    tokenList[tokenIndex] = token;
                    tokenIndex++;
                }
                
            }
            
        }
        //  check to see if the current character is a number
        else if(isdigit(currentIndexCharacter)){
            //  prints first digit to cleaninput file
            //printInput(currentIndexCharacter);
            
            //  creates and places first number in possible word array
            char identifier[IDENTIFIER_MAX_LENGTH];
            identifier[i] = currentIndexCharacter;
            i++;
            
            //  gets next letter and prints it to cleaninput if not a period comma or semicolon
            currentIndexCharacter = fgetc(input);
            if (currentIndexCharacter != ',' && currentIndexCharacter != ';' && currentIndexCharacter != '.'){
                //printInput(currentIndexCharacter);
            }
            
            //  loop to keep getting next char as long as it is a number
            while (isdigit(currentIndexCharacter)) {
                identifier[i] = currentIndexCharacter;
                i++;
                currentIndexCharacter = fgetc(input);
                if (currentIndexCharacter != ',' && currentIndexCharacter != ';' && currentIndexCharacter != '.'){
                   // printInput(currentIndexCharacter);
                }
            }
            
            //  error check to see if the variable started with a number
            if (isalpha(currentIndexCharacter)) {
                error(1);
            }
            //  check to see if the number is to large
            else if (i > NUMBER_MAX_LENGTH){
                error(2);
            }
            //  store the token
            //  unget last char and place ending terminating symbol
            else{
                ungetc(currentIndexCharacter, input);
                identifier[i] = '\0';
                
                token.tokenID = numbersym;
                strcpy(token.name, identifier);
                token.index = tokenIndex;
                tokenList[tokenIndex] = token;
                tokenIndex++;
            }
            
            
        }
        //  check for if the current symbol is a special symbol or not
        else if ((specialSymbolChar = isSpecialChar(currentIndexCharacter)) >= 0){
            //  switch statements for the differet special symbols
            switch (specialSymbolChar) {
                //  if reading in colon it becomes becomes symbol
                case becomessym:
                {
                    //printInput(currentIndexCharacter);
                    currentIndexCharacter = fgetc(input);
                    if ((specialSymbolChar = isSpecialChar(currentIndexCharacter)) == eqsym) {
                        token.tokenID = becomessym;
                        strcpy(token.name, ":=");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                        //printInput(currentIndexCharacter);
                    }
                    //  if colon is not followed by equals than an error occurs
                    else{
                        ungetc(currentIndexCharacter, input);
                        error(4);
                    }
                    break;
                }
                //  case for less than symbol
                case lessym:
                {
                    //printInput(currentIndexCharacter);
                    
                    currentIndexCharacter = fgetc(input);
                    
                    //  different checks to see if less than symbol followed by greater than or equals sym to add current symbol to tokens
                    //  else it ungets current char and adds less than to token list
                    if ((specialSymbolChar = isSpecialChar(currentIndexCharacter)) == gtrsym) {
                        token.tokenID = neqsym;
                        strcpy(token.name, "<>");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                        //printInput(currentIndexCharacter);
                    }
                    else if (specialSymbolChar == eqsym){
                        token.tokenID = leqsym;
                        strcpy(token.name, "<=");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                        //printInput(currentIndexCharacter);
                    }
                    else{
                        ungetc(currentIndexCharacter, input);
                        token.tokenID = lessym;
                        strcpy(token.name, "<");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                    }
                    break;
                }
                //  check for greater than sym
                case gtrsym:
                {
                    //printInput(currentIndexCharacter);
                    currentIndexCharacter = fgetc(input);
                    
                    //  different checks to see if greater than symbol followed by equals sym to add current symbol to tokens
                    //  else it ungets current char and adds greater than to token list
                    if ((specialSymbolChar = isSpecialChar(currentIndexCharacter)) == eqsym) {
                        token.tokenID = geqsym;
                        strcpy(token.name, ">=");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                        //printInput(currentIndexCharacter);
                    }
                    else{
                        ungetc(currentIndexCharacter, input);
                        token.tokenID = gtrsym;
                        strcpy(token.name, ">");
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                    }
                    break;
                }
                //  case for the slash special symbol
                case slashsym:
                {

                    //currentIndexCharacter = fgetc(input);
                    char temp = fgetc(input);
                    
                    //  if comment handle accordingly
                    if (temp == '*') {
                        while ((temp = fgetc(input)) != EOF) {
                            //  if possible end to comments
                            if (temp == '*') {
                                temp = fgetc(input);
                                //  comments are over
                                if (temp == '/') {
                                    break;
                                }
                                else{
                                    ungetc(temp, input);
                                }
                            }
                        }
                    }
                    //  no comments so add slash symbol to tokens
                    else{
                        ungetc(temp, input);

                        token.tokenID = specialSymbolChar;
                        
                        token.name[0] = currentIndexCharacter;
                        token.name[1] = '\0';
                        token.index = tokenIndex;
                        tokenList[tokenIndex] = token;
                        tokenIndex++;
                        //printInput(currentIndexCharacter);
                    }
                    
                    break;
                }
                //  no above cases so special symbol gets added to tokens as is
                default:
                    token.tokenID = specialSymbolChar;
                    
                    token.name[0] = currentIndexCharacter;
                    token.name[1] = '\0';
                    token.index = tokenIndex;
                    tokenList[tokenIndex] = token;
                    tokenIndex++;
                    //printInput(currentIndexCharacter);
                    break;
            }
        }
        //  if its an invisiable character print it to the clean input
        //  do nothing else
        else if (isInvisibleChar(currentIndexCharacter)){
            //printInput(currentIndexCharacter);
        }

        else{
            error(4);
        }
    }
    
    //  function calls to print the lexemetable and lexemelist
    //printLexemeTable();
    
    
    printLexemeList();
    
    //  closes respective files
    fclose(input);
    //fclose(cleanInput);
    //fclose(lexemeTable);
    fclose(lexemeList);
    
    return 0;
}

