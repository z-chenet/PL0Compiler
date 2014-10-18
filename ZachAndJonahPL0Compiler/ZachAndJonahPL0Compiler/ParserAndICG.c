
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 The Parser is a program that reads in the output of the Scanner (HW2) and parses the lexemes (tokens). It must be capable of reading in the tokens produced by your Scanner (HW2) and produce, as output, a message that states whether the PL/0 program is well-formed (syntactically correct) if it follows the grammar rules in Appendix B. Otherwise, if the program does not follow the grammar, a message indicating the type of error present must be printed. A list of the errors to be considered can be found in Appendix C. In addition, the Parser must generate the Symbol Table, which contains all of the variables, procedure names and constants within the PL/0 program. See Appendix E for more information regarding the Symbol Table. If the program is syntactically correct and the Symbol Table is created without error, the execution of the compiler driver continues with intermediate code generation.

 The Intermediate Code Generator is a program that takes, as input, the output from the Parser, i.e. the Symbol Table and parsed code. As output, it produces the assembly language for your Virtual Machine (HW1). This functionality will be interleaved with the Parser functionality (i.e. generate assembly code as you parse the token list). Once the code has been generated for your Virtual Machine, the execution of the compiler driver continues by executing the generated assembly code on your Virtual Machine (HW1).

*********/


/***************************************************************************/
/*                       Procedure and call not supported                  */
//  after checking that program ends with perioid call HALT

#include "ParserAndICG.h"

void Parser(Token tokenList[MAX_FILE_SIZE]) {
    /*
        - We have an array of tokens. Refer to Scanner.h for the Token definition
        - We need to go token by token till the end of line creating the symbol table and checking for grammar errors
     */
    
    //getToken();
    //block();
    run();
    
    if (errorOccured == 0) {
        printf("No errors, program is syntactically correct.\n");
    }
    
    
    
    
}

void getToken(){
    currentToken = tokenList[currentTokenTableIndex];
    currentTokenTableIndex++;
}

void run(){
    getToken();
    
    block();
    emit(11, 0, 3);
    
    if (currentToken.tokenID != periodsym) {
        parserErrors(9);
    }

}

void block(){
    int tempBlockIndex = currentMCodeTableIndex;
    
    //  because first call is always jump
    //emit(7, 0, 0);

    if (currentToken.tokenID == constsym) {
        constDeclaration();
    }
    
    if (currentToken.tokenID == varsym) {
        varDeclaration();
    }
    
    int tempMaddress = currentMAddress;

    while (currentToken.tokenID == procsym) {
        procDeclaration();
    }
    
    mCodeTable[tempBlockIndex].M = currentMAddress;
    
    emit(6, 0, tempMaddress);
    
    statement();
    
    //emit(11, 0, 3);
    
    
}

void emit(int op, int l, int m){
    mCodeTable[currentMCodeTableIndex].OP = op;
    mCodeTable[currentMCodeTableIndex].L = l;
    mCodeTable[currentMCodeTableIndex].M = m;
    
    currentMCodeTableIndex++;
    
}

void constDeclaration(){
    do {
        Token tempToken;
        
        
        getToken();
        if (currentToken.tokenID != identsym) {
            parserErrors(4);
        }
        
        strcpy(tempToken.name, currentToken.name);
        
        getToken();
        if (currentToken.tokenID != eqsym) {
            parserErrors(3);
        }
        
        getToken();
        if (currentToken.tokenID != numbersym) {
            parserErrors(2);
        }
        
        enterInSymbolTable(1, tempToken, lexiLevel, -5, convertToInt(currentToken.name));
        getToken();
        
        
    } while (currentToken.tokenID == commasym);
    
    if (currentToken.tokenID != semicolonsym) {
        parserErrors(5);
    }
    
    getToken();
}

void varDeclaration(){
    do {
        getToken();
        if (currentToken.tokenID != identsym) {
            parserErrors(4);
        }
        
        enterInSymbolTable(2, currentToken, lexiLevel, currentMAddress , 0);
        
        getToken();
        
    } while (currentToken.tokenID == commasym);
    
    if (currentToken.tokenID != semicolonsym) {
       
        parserErrors(5);
    }
    
    getToken();
}

void procDeclaration(){
    getToken();
    if (currentToken.tokenID != identsym) {
        parserErrors(6);
    }
    
    lexiLevel++;
    
    //  +1 for PC
    enterInSymbolTable(3, currentToken, lexiLevel, currentMCodeTableIndex + 1, -1);
    
    getToken();
    if (currentToken.tokenID != semicolonsym) {
        parserErrors(6);
    }
    
    getToken();
    
    block();
    lexiLevel--;
    
    if (currentToken.tokenID != semicolonsym) {
        parserErrors(5);
    }
    getToken();
}

void statement(){
    if (currentToken.tokenID == identsym) {
        printf("%d", currentToken.tokenID);
        printf("%s", currentToken.name);
        
        int symbolLocation = searchSymbolTableForIdentifier(currentToken.name);
        printf("%d\n", symbolLocation);
        
        /*error occuring here*/
        if (symbolLocation == -1) {
            printf("3:: %d\n\n", currentToken.tokenID);
            parserErrors(11);
        }
        if (symbol_table[symbolLocation].kind == 1) {
            parserErrors(12);
        }
        
        int ident_addr;
        ident_addr = symbol_table[symbolLocation].addr;
        
        getToken();
        
        if (currentToken.tokenID != becomessym) {
            if (currentToken.tokenID == eqsym) {
                parserErrors(1);
            }
            parserErrors(13);
            
        }
        
        getToken();
        expression();
        
        if (currentToken.tokenID != semicolonsym) {
            parserErrors(10);
        }
        
        emit(4, lexiLevel - symbol_table[symbolLocation].level, ident_addr);
    }
    else if (currentToken.tokenID == callsym){
        getToken();
        
        int symbol_index = searchSymbolTableForIdentifier(currentToken.name);
        
        if (symbol_index == -1) {
            parserErrors(11);
        }
        
        if (currentToken.tokenID != identsym) {
            parserErrors(11);
        }
        
        getToken();
        
        emit(5, lexiLevel, symbol_table[symbol_index].addr);
    }
    else if (currentToken.tokenID == beginsym){
        getToken();
        statement();
        
        while (currentToken.tokenID == semicolonsym) {
            getToken();
            statement();
            //  after statement symbol is 32 for some reason
        }
        //printf("TEST HERE :: %d\n\n", currentToken.tokenID);
        //  error here
        if (currentToken.tokenID != endsym) {
            parserErrors(17);
        }
        
        getToken();
        //finishedProcedure(lexiLevel);
    }
    else if (currentToken.tokenID == ifsym){
        getToken();
        condition();
        
        if (currentToken.tokenID != thensym) {
            parserErrors(16);
        }
        
        getToken();
        int tempBlockIndex = currentMCodeTableIndex;
        
        emit(8, 0, 0);
        
        statement();
        mCodeTable[tempBlockIndex].M = currentMCodeTableIndex;
        
        if (currentToken.tokenID == elsesym) {
            mCodeTable[tempBlockIndex].M = currentMCodeTableIndex++;
            tempBlockIndex = currentMCodeTableIndex;
            emit(7, 0, 0);
            getToken();
            statement();
            mCodeTable[tempBlockIndex].M = currentMCodeTableIndex;
        }
        
    }
    else if (currentToken.tokenID == whilesym){
        int tempIndex1 = currentMCodeTableIndex, tempIndex2;
        
        getToken();
        condition();
        
        tempIndex2 = currentMCodeTableIndex;
        
        emit(8, 0, 0);
        
        if (currentToken.tokenID != dosym) {
            parserErrors(18);
        }
        
        getToken();
        statement();
        
        emit(7, 0, tempIndex1);
        
        mCodeTable[tempIndex2].M = currentMCodeTableIndex;
    }
    else if (currentToken.tokenID == readsym){
        getToken();
        
        if (currentToken.tokenID == identsym) {
            int symbol_index = searchSymbolTableForIdentifier(currentToken.name);
            //printf("TESTING :: %d\n", symbol_index);
            if (symbol_index == -1) {
                parserErrors(11);
            }
            getToken();
            
            //  adds the read in statement to the mcode
            emit(10, 0, 2);
            
            //  adds the increment statement to the mcode
            emit(4, 0, symbol_table[symbol_index].addr);
            
            //emit(3, 0, symbol_table[symbol_index].addr);
        }
        
    }
    else if (currentToken.tokenID == writesym){
        getToken();
        
        if (currentToken.tokenID == identsym) {
            int symbol_index = searchSymbolTableForIdentifier(currentToken.name);
            
            if (symbol_index == -1) {
                parserErrors(11);
            }
            
            getToken();
            
            //  loads the value write to console
            emit(3, 0, symbol_table[symbol_index].addr);
            
            //  adds output statement to the mcode
            emit(9, 0, 1);
        }
    }
    
}

void expression(){
    int currentOperation;

    if (currentToken.tokenID == plussym || currentToken.tokenID == minussym) {

        currentOperation = currentToken.tokenID;
        
        if (currentOperation == minussym) {
            emit(2, 0, 1);
        }
        
    }
    term();
    
    while (currentToken.tokenID == plussym || currentToken.tokenID == minussym) {
        currentOperation = currentToken.tokenID;
        getToken();
        term();
        if (currentOperation == plussym) {
            emit(2, 0, 2);
            
        }
        else{
            emit(2, 0, 3);
        }
    }
    
}

void term(){
    int currentOperation;
    
    factor();
    
    while (currentToken.tokenID == multsym || currentToken.tokenID == slashsym) {
        currentOperation = currentToken.tokenID;
        getToken();
        factor();
        if (currentOperation == multsym) {
            emit(2, 0, 4);
        }
        else{
            emit(2, 0, 5);
        }
    }
}

void factor(){
    if (currentToken.tokenID == identsym) {
        int symbolIndex = searchSymbolTableForIdentifier(currentToken.name);
        
        if (symbolIndex == -1) {
            parserErrors(11);
        }
        
        if (symbol_table[symbolIndex]. kind == 1) {
            emit(1, 0, symbol_table[symbolIndex].val);
        }
        else{
            emit(3, lexiLevel - symbol_table[symbolIndex].level, symbol_table[symbolIndex].addr);
        }
        
        getToken();
    }
    else if (currentToken.tokenID == numbersym){
        emit(1, 0, convertToInt(currentToken.name));
        getToken();
    }
    else if (currentToken.tokenID == lparentsym){
        getToken();
        expression();
        
        if (currentToken.tokenID != rparentsym) {
            parserErrors(22);
        }
        
        getToken();
    }
    else{
        parserErrors(23);
    }

}

void condition(){
    int currentOperation;
    
    if (currentToken.tokenID == oddsym) {
        emit(2, 0, 6);
        getToken();
        expression();
    }
    else{
        expression();
        
        currentOperation = currentToken.tokenID;
        
        //  checking for all different kind of relations
        if (currentToken.tokenID == eqsym) {
            currentOperation = 8;
        }
        
        switch (currentToken.tokenID) {
            case eqsym:
                currentOperation = 8;
                break;
            case neqsym:
                currentOperation = 9;
                break;
            case lessym:
                currentOperation = 10;
                break;
            case leqsym:
                currentOperation = 11;
                break;
            case gtrsym:
                currentOperation = 12;
                break;
            case geqsym:
                currentOperation = 13;
                break;
            default:
                parserErrors(20);
                break;
        }
        
        getToken();
        expression();
        emit(2, 0, currentOperation);
        
    }
}

int searchSymbolTableForIdentifier(char *identifier){
    
    int i;
    for (i = currentSymbolTableIndex - 1; i >= 0; i--) {
        if (strcmp(identifier, symbol_table[i].name) == 0 && symbol_table[i].addr != -1) {
            return i;
        }
    }
    
    return -1;
}

void enterInSymbolTable(int type, Token token, int l, int m, int value){
    symbol_table[currentSymbolTableIndex].kind = type;
    strcpy(symbol_table[currentSymbolTableIndex].name, token.name);
    symbol_table[currentSymbolTableIndex].level = l;
    symbol_table[currentSymbolTableIndex].addr = m;
    if (type == 1) {
        symbol_table[currentSymbolTableIndex].val = value;
    }
    else if (type == 2){
        currentMAddress++;
    }
    
    currentSymbolTableIndex++;
}

void parserErrors(int code){
    errorOccured = 1;
    
    int i;
    for(i=0;i<currentMCodeTableIndex;i++) {
        printf("%d %d %d\n",mCodeTable[i].OP,mCodeTable[i].L,mCodeTable[i].M);
    }
    
    switch (code) {
        case 1:
            printf("Use = instead of :=.\n");
            break;
        case 2:
            printf("= must be followed by a number\n");
            break;
        case 3:
            printf("Identifier must be followed by =.\n");
            break;
        case 4:
            printf("const, int, procedure must be followed by identifier\n");
            break;
        case 5:
            printf("Semicolon or comma missing\n");
            break;
        case 6:
            printf("Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("Statement expected\n");
            break;
        case 8:
            printf("Incorrect symbol afer statement part in block\n");
            break;
        case 9:
            printf("Period expected.\n");
            break;
        case 10:
            printf("Semicolon between statements missing\n");
            break;
        case 11:
            printf("Undeclared identifier\n");
            break;
        case 12:
            printf("Assignment to constant or procedure is not allowed\n");
            break;
        case 13:
            printf("Assignment operator expected\n");
            break;
        case 14:
            printf("call must be followed by an identifier\n");
            break;
        case 15:
            printf("Call of a constant or variable is meaningless\n");
            break;
        case 16:
            printf("then expected\n");
            break;
        case 17:
            printf("Semicolon or } expected\n");
            break;
        case 18:
            printf("do expected\n");
            break;
        case 19:
            printf("Incorrect symbol following statement\n");
            break;
        case 20:
            printf("Relational operator expected\n");
            break;
        case 21:
            printf("Expression must not contain a procedure identifier\n");
            break;
        case 22:
            printf("Right parenthesis missing\n");
            break;
        case 23:
            printf("The preceding factor cannot begin with this symbol\n");
            break;
        case 24:
            printf("An expression cannot begin with this symbol\n");
            break;
        case 25:
            printf("This number is too large\n");
            break;
          default:
            break;
    }
    
    exit(1);
}

int convertToInt(char *number){
    int temp = 0, i = 0;
    while (number[i] != '\0')
    {
        temp *= 10;
        temp += number[i] - '0';
        i++;
    }
    return temp;
}

//  this isnt really needed it was causing the second sample input to crash because it was removing things from the symbol table that was still needed
/*
void finishedProcedure(int level){
    printf("entered here\n");
    int i;
    for (i = currentSymbolTableIndex - 1; i >= 0; i--) {
        if (symbol_table[i].level == level && symbol_table[i].kind != 3) {
            symbol_table[i].addr = -1;
        }
    }
}*/


void printMcodeToFile(FILE* mcodeOutput) {

    int i;
    for(i=0;i<currentMCodeTableIndex;i++) {
        fprintf(mcodeOutput,"%d %d %d\n",mCodeTable[i].OP,mCodeTable[i].L,mCodeTable[i].M);
    }
}











