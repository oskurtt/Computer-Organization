#include<stdio.h>
#include<stdlib.h>
#include <string.h>

//struct to store each mip line instruction data
typedef struct {
    int instructions[9];
    int count;
    int isnop;
    int isnop_count;
}Data;

//print out nop lines
void print_nop(int* nop, int cycle_count){
    int counter = 0;
    for (int j = 0; j < 9; j++){
        //when all 5 instructions are executed, only print "."
        if (counter >= 5){
            printf("%c", '.');
        }
        else{
            if ((nop[j] >= 0) && (j == 0)){
                counter++;
            }
            //if there is a duplicate instruction, don't count that towards counter
            else if (j > 0){
                if ((nop[j] != nop[j-1]) || nop[j] == 5){
                    counter++;
                }
            }

            //print symbol based on number
            if (nop[j] == -1){
                printf("%c", '.');
            }
            if (nop[j] == 5){
                if (j < cycle_count){
                    printf("%c", '*'); 
                }
                else printf("%c", '.');
            }
            if (nop[j] == 0){
                printf("%s", "IF");
            }
            if (nop[j] == 1){
                printf("%s", "ID");
            }
            if (nop[j] == 2){
                printf("%s", "EX");
            }
            if (nop[j] == 3){
                printf("%s", "MEM");
            }
            if (nop[j] == 4){
                printf("%s", "WB");
            }
        }
        if (j == 8){
            printf("\n");
            break;
        }
        printf("\t");
    }
    return;
}
//print mip lines
void print_mip(int* mip, int cycle_count){
    for (int j = 0; j < 9; j++){
        if (mip[j] == -1){
            printf("%c", '.');
        }
        else if (mip[j] == 0){
            printf("%s", "IF");
        }
        else if (mip[j] == 1){
            printf("%s", "ID");
        }
        else if (mip[j] == 2){
            printf("%s", "EX");
        }
        else if (mip[j] == 3){
            printf("%s", "MEM");
        }
        else if (mip[j] == 4){
            printf("%s", "WB");
        }
        else{
            printf("%c", '.');
        }
        if (j == 8){
            printf("\n");
            break;
        }
        printf("\t");
    }
    return;
}

int main(int argc, char* argv[]) {
    // Open input file
    char const* const file_name = argv[1];
    FILE * file;
    file = fopen(file_name, "r");

    //variables
    int linecount = 0;
    int cycle_count = 0;
    int i = 0;
    int j = 0;
    char lines[5][128];

    int nops1[9];
    int nops2[9];
    Data mips[5];
    char dependency[3];

    //initialize everything in nops to -1 which equals to "."
    for (i = 0; i < 9; i++){
            nops1[i] = -1;
            nops2[i] = -1;
    }

    //initialize our struct for a total of 5 lines of mips
    for (i = 0; i < 5; i++){
        for (j = 0; j < 9; j++){
            mips[i].instructions[j] = -1;
        }
        mips[i].count = 0;
        mips[i].isnop = -1;
        mips[i].isnop_count = 1;
    }

    //get each line of code from input file
    while (fgets(lines[linecount], sizeof(lines[linecount]), file)) {
        i = 0;
        while (lines[linecount][i] != '\n'){
            i++;
        }
        lines[linecount][i] = '\t';
        //lines[linecount][i+1] = '\0';
        linecount++;
    }
    
    //find the dependency
    for (i = 0; lines[0][i] != '\0'; i++){
        if (lines[0][i] == '$'){
            dependency[0] = lines[0][i+1];
            dependency[1] = lines[0][i+2];
            dependency[2] = '\0';
            break;
        }
    }

    //checks which line of code relies on a dependency
    for (i = 1; i < linecount; i++){
        for (j = 0; lines[i][j] != '\0'; j++){
            if (lines[i][j] == '$'){
                if ((lines[i][j+1] == dependency[0]) && (lines[i][j+2] == dependency[1])){
                    mips[i].isnop = 1;
                }
            }
        }
    }
 
    printf("START OF SIMULATION\n\n");
    for (cycle_count = 1; cycle_count < 10; cycle_count ++){
        //if the last lines finishes executing 5 instructions, break
        if (mips[linecount-1].count == 5) break;
        //title
        printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
    
        for (i = 0; i < linecount; i++){
            //cycle 1 
            if (cycle_count == 1){
                printf("%s", lines[i]);
                if (i == 0){
                    printf("IF\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
                else{
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }
            //cycle 2
            else if (cycle_count == 2) {
                printf("%s", lines[i]);
                if (i == 0) {
                    printf("IF\tID\t.\t.\t.\t.\t.\t.\t.\n");
                }
                else if (i == 1) {
                    printf(".\tIF\t.\t.\t.\t.\t.\t.\t.\n");
                    mips[i].count++;
                } 
                else{
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }
            //cycle 3 
            else if (cycle_count == 3) {
                printf("%s", lines[i]);
                if (i == 0) {
                    printf("IF\tID\tEX\t.\t.\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf(".\tIF\tID\t.\t.\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    printf(".\t.\tIF\t.\t.\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
                else{
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }
            //cycle 4
            else if (cycle_count == 4) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\t.\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    //if there are 2 dependencies
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 1) && (mips[2].isnop == 1)){
                        printf("nop\t\t");
                        //initialize values for first nop based on current instructions being executed 
                        for (j = 0; j < mips[i].count; j++){
                            nops1[1+j] = j;
                            mips[i].instructions[1+j] = j;
                        }
                        //set the rest of the nop instructions to "*"
                        for (j = mips[i].count+1; j < 9; j++){
                            nops1[j] = 5;
                        }
                        //duplicate previous instruction because there is a hazard
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];

                        //print nop and mips
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\t.\t.\t.\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 2) {
                    //if there are 2 dependencies
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 2)){
                        //set values for mips and print them
                        for (j = 0; j < mips[i].count; j++){
                            mips[i].instructions[2+j] = j;
                        }
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);

                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                        mips[i].count++;
                    }
                    
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\t.\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
                else{
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }
            //cycle 5
            else if (cycle_count == 5) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    //if 2 dependencies, create a second nop 
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 2) && (mips[2].isnop == 1)){
                        printf("nop\t\t");
                        print_nop(nops1, cycle_count);
                        //copy over the values of nops1 over to nops2
                        for (j = 0; j < 9; j++){
                            nops2[j] = nops1[j];
                        }
                        //duplicate previous instruction
                        nops2[cycle_count-2] = nops2[cycle_count-3];
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];
                        //print nops2 and mips
                        printf("nop\t\t");
                        print_nop(nops2, cycle_count);
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].isnop_count++;
                    }

                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\tMEM\t.\t.\t.\t.\n");
                        mips[i].count++;
                    }
                    
                }
                else if (i == 2) {
                    //duplicate previous instruction and print
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 3)){
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                    }
                    //if only line 2 have a dependency
                    else if ((mips[i].isnop == 1) && (mips[i].isnop_count == 1)){
                        //initialize values for nops2
                        printf("nop\t\t");
                        for (j = 0; j < mips[i].count; j++){
                            nops1[2+j] = j;
                            mips[i].instructions[2+j] = j;
                        }
                        //set rest of nops to "*"
                        for (j = mips[i].count+2; j < 9; j++){
                            nops1[j] = 5;
                        }
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];
                        //print
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                        mips[i].count++;
                        
                    }
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\tID\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 4) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\tIF\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
            } 
            //after cycle 5, first line will finish running
            //cycle 6
            else if (cycle_count == 6) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 3) && (mips[2].isnop == 1) ){
                        //print nops based on remaining cycle
                        printf("nop\t\t");
                        print_nop(nops1, cycle_count);
                        printf("nop\t\t");
                        print_nop(nops2, cycle_count);
                        //update mips instruction and print
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        mips[i].count++; 
                    }
                    
                }
                else if (i == 2) {
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 4)){
                        //update and print
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }

                    else if (mips[i].isnop == 1 && mips[i].isnop_count == 2){
                        //update and print
                        printf("nop\t\t");
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);

                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\tMEM\t.\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\tID\tEX\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 4) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\tIF\tID\t.\t.\t.\n");
                    mips[i].count++;
                }
            }
            //cycle 7
            else if (cycle_count == 7) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    //update and print
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 4) && (mips[2].isnop == 1) ){
                        printf("nop\t\t");
                        print_nop(nops1, cycle_count);
                        printf("nop\t\t");
                        print_nop(nops2, cycle_count);
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        mips[i].count++;
                    }
                    
                }
                else if (i == 2) {
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 5)){
                        //update and print
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }

                    else if (mips[i].isnop == 1 && mips[i].isnop_count == 3){
                        //update and print
                        printf("nop\t\t");
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);

                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\tID\tEX\tMEM\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 4) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\tIF\tID\tEX\t.\t.\n");
                    mips[i].count++;
                }
            }
            //8
            else if (cycle_count == 8) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    //update and print
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 5) && (mips[2].isnop == 1) ){
                        printf("nop\t\t");
                        print_nop(nops1, cycle_count);
                        printf("nop\t\t");
                        print_nop(nops2, cycle_count);
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 2) {
                    //update and print
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 6)){
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }

                    else if (mips[i].isnop == 1 && mips[i].isnop_count == 4){
                        //update and print
                        printf("nop\t\t");
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);

                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                    mips[i].count++;
                }
                else if (i == 4) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\t.\n");
                    mips[i].count++;
                }
            }
            //cycle 9
            else if (cycle_count == 9) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    //update and print
                    if ((mips[i].isnop == 1) && (mips[i].isnop_count == 6) && (mips[2].isnop == 1) ){
                        printf("nop\t\t");
                        print_nop(nops1, cycle_count);
                        printf("nop\t\t");
                        print_nop(nops2, cycle_count);
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        mips[i].count++; 
                    }
                    
                }
                else if (i == 2) {
                    //update and print
                    if ((mips[1].isnop == 1) && (mips[i].isnop == 1) && (mips[1].isnop_count == 7)){
                        mips[i].instructions[cycle_count-1] = mips[i].count;
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 3) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                    mips[i].count++;
                }
                else if (i == 4) {
                    printf("%s", lines[i]);
                    printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\tWB\n");
                    mips[i].count++;
                }
            }

            else{
                printf("%s", lines[i]);
                printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
            }

        }
        printf("\n");
    }
    //close file and end simulation
    fclose(file);
    printf("END OF SIMULATION\n");
    return EXIT_SUCCESS;
}