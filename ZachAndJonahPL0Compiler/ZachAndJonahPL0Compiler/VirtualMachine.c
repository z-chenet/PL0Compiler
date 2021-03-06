
/*********
 
 Jonah Katz and Zachary Chenet
 
 University of Central Florida
 COP 3402: System Software
 Fall 2014
 
 Homework #3 (Parser - Code Generator)
 
 *********/


#include "ParserAndICG.c"

//  Constants
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3

//  arrays to keep track of different instructions and M operations
//  index zero is empty because instructions start on 1
const char* OPCODE_STRING[] = {"N/A", "LIT", "OPR", "LOD", "STO", "CAL",
                                "INC", "JMP", "JPC", "SIO", "SIO", "SIO"};
const char* STACK_OP_STRING[] = {"RET", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD",
                                        "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
//  struct for the instructions
/*typedef struct{
    int OP;
    int L;
    int M;
}instruction;*/

//  global arrays
int stack[MAX_STACK_HEIGHT];
instruction code[MAX_CODE_LENGTH];
int basePointers[MAX_STACK_HEIGHT]; //  make value larger to accomodate many activation record

//  registers
int SP = 0;
int BP = 1;
int PC = 0;
instruction IR;

//  global variables
int codeLength;
int halt = 0;
int levels;
//FILE* input;
FILE* output;
FILE* stackTraceOutput;

//  functions
void printCodeInputs();
void printCurrentCode(int line);
void printStackElements();
void execute(int shouldPrintStackTrace);
void fetchExecuteInstruction();
void stackExecutions();
void handleInputOutput();
int base(int l,int base);

//  main
int VirualMachine(FILE *input, int shouldPrintStackTrace)
{
    //  initializes values
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    
    //input = fopen("mcode.txt", "r");
    //(if input){
    //  local variables to keep track of inputs and values saved
    int OP_code;
    int L_code;
    int M_code;
    
    //  variable to keep track of how long code is
    int i = 0;
    
    //  loop to go through file and save OP, M, and L value
    //  while loop stops at end of file
    while (fscanf(input, "%d", &OP_code) != EOF) {
        fscanf(input, "%d", &L_code);
        fscanf(input, "%d", &M_code);
        
        code[i].OP = OP_code;
        code[i].L = L_code;
        code[i].M = M_code;
        
        
        i++;
    }
    
    codeLength = i;
 
    output = fopen("stacktrace.txt", "w");
    
    
    //  prints the code that is being input
    printCodeInputs();
    
    //  runs the code
    
    //prepare temp stacktarce file
    stackTraceOutput = fopen("stacktraceTemp.txt", "w");
    execute(shouldPrintStackTrace);
    fclose(output);
    fclose(stackTraceOutput);
    

        
    
    return 0;
}

//  function to handle printing the list of inputted code in OP L M format
void printCodeInputs(){
    fprintf(output, "Stacktrace Code:\n\n");
    int i;
    fprintf(output, "Line\t OP\t L\t  M\n");
    for (i = 0; i < codeLength; i++) {
        printCurrentCode(i);
        fprintf(output, "\n");
    }
}

//  function call to print out the current indexed code
void printCurrentCode(int line){
    fprintf(output, "%3d\t%s\t%2d\t%3d", line, OPCODE_STRING[code[line].OP], code[line].L, code[line].M);
}

//  function to print the stack elements of the current indexed instruction
void printStackElements(int shouldPrintStackTrace){

    levels = 0;
    
    int i;
    for (i =  1; i <= SP; i++) {
        //  check to see if at end of activation record
        //  if so add its index to the basepointers array
        if (i == BP && BP != 1){
            fprintf(output, "| ");
            if(shouldPrintStackTrace == 1) {
                fprintf(stackTraceOutput, "| ");
            }
            basePointers[levels] = i;
        }
        else if(i == basePointers[levels]){
            if(shouldPrintStackTrace == 1) {
                fprintf(stackTraceOutput, "| ");
            }
            fprintf(output, "| ");
            levels++;
        }
        if(shouldPrintStackTrace == 1) {
            fprintf(stackTraceOutput, "%d ", stack[i]);
        }

        fprintf(output, "%d ", stack[i]);
    }
    if(shouldPrintStackTrace == 1) {
        fprintf(stackTraceOutput, "\n");
    }
    fprintf(output, "\n");
}

//  function to execute the given set of instructions
void execute(int shouldPrintStackTrace){
    if(shouldPrintStackTrace == 1) {
        fprintf(stackTraceOutput, "\nStacktrace:\n");
        fprintf(stackTraceOutput, "\t  PC\t\t BP\t\t SP\t\tStack\n");
        fprintf(stackTraceOutput, "\t %2d\t\t %2d\t\t%3d", PC, BP, SP);
    }
    
    fprintf(output, "\nStacktrace:\n");
    fprintf(output, "\t\t\t\t  PC\t\t BP\t\t SP\t\tStack\n");
    fprintf(output, "Initial Values\t\t\t %2d\t\t %2d\t\t%3d", PC, BP, SP);
    printStackElements(shouldPrintStackTrace);
    
    while (BP > 0 && halt == 0) {
        if (PC < codeLength) {
            printCurrentCode(PC);
            fetchExecuteInstruction();
            
            fprintf(output, "\t %2d\t\t %2d\t\t%3d\t\t", PC, BP, SP);
            if(shouldPrintStackTrace == 1) {
                fprintf(stackTraceOutput, "\t %2d\t\t %2d\t\t%3d\t\t", PC, BP, SP);
            }
            printStackElements(shouldPrintStackTrace);
        }
    }
}



//  function to execute the next I
void fetchExecuteInstruction(){
    //  fetch cycle
    IR = code[PC];
    PC = PC +1;
    
    //  execute cycle
    //  switch statement to handle different OP codes
    switch (IR.OP) {
        case 0:
            break;
        case 1:
            SP = SP + 1;
            stack[SP] = IR.M;
            break;
        case 2:
            stackExecutions();
            break;
        case 3:
            SP = SP + 1;
            stack[SP] = stack[base(IR.L, BP) + IR.M];
            break;
        case 4:
            stack[base(IR.L, BP) + IR.M] = stack[SP];
            SP = SP - 1;
            break;
        case 5:
            stack[SP + 1] = 0;
            stack[SP + 2] = base(IR.L, BP);
            stack[SP + 3] = BP;
            stack[SP + 4] = PC;
            BP = SP + 1;
            PC = IR.M;
            break;
        case 6:
            SP = SP + IR.M;
            break;
        case 7:
            PC = IR.M;
            break;
        case 8:
            if (stack[SP] == 0) {
                PC = IR.M;
            }
            SP = SP - 1;
            break;
        case 9:
            handleInputOutput();
            break;
        case 10:
            handleInputOutput();
            break;
        case 11:
            handleInputOutput();
            break;
            
        default:
            break;
    }
}

//  function to Operate on the stack depending on what the M value is
void stackExecutions(){
    //  switch statement to handle different M codes for OPR instruction
    switch (IR.M) {
        case 0:
            SP = BP - 1;
            PC = stack[SP + 4];
            BP = stack[SP+3];
            break;
        case 1:
            stack[SP] = -stack[SP];
            break;
        case 2:
            SP = SP - 1;
            stack[SP] = stack[SP] + stack[SP + 1];
            break;
        case 3:
            SP = SP - 1;
            stack[SP] = stack[SP] - stack[SP + 1];
            break;
        case 4:
            SP = SP - 1;
            stack[SP] = stack[SP] * stack[SP + 1];
            break;
        case 5:
            SP = SP - 1;
            stack[SP] = stack[SP] / stack[SP + 1];
            break;
        case 6:
            stack[SP] = stack[SP] % 2;
            break;
        case 7:
            SP = SP - 1;
            stack[SP] = stack[SP] % stack[SP + 1];
            break;
        case 8:
            SP = SP - 1;
            stack[SP] = stack[SP] == stack[SP + 1];
            break;
        case 9:
            SP = SP - 1;
            stack[SP] = stack[SP] != stack[SP + 1];
            break;
        case 10:
            SP = SP - 1;
            stack[SP] = stack[SP] < stack[SP + 1];
            break;
        case 11:
            SP = SP - 1;
            stack[SP] = stack[SP] <= stack[SP + 1];
            break;
        case 12:
            SP = SP - 1;
            stack[SP] = stack[SP] > stack[SP + 1];
            break;
        case 13:
            SP = SP - 1;
            stack[SP] = stack[SP] >= stack[SP + 1];
            break;
            
        default:
            break;
    }
}

//  function to handle the different M values for the SIO instruction
void handleInputOutput(){
    int temp = 0;
    
    //  switch statement to handle different M codes for InputOutput
    switch (IR.M) {
        case 0:
            break;
        case 1:
            printf("(%d)\n\n", stack[SP]);
            SP = SP - 1;
            break;
        case 2:
            SP = SP + 1;
            scanf("%d", &temp);
            stack[SP] = temp;
            break;
        case 3:
            halt = 1;
            break;
            
        default:
            break;
    }
}

//  given function on how to go into deeper activation records
int base(int l, int base){
    int b1;
    b1 = base;
    while (l > 0) {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}

