#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct {
    int instructions[9];
    int count;
    int isnop;
    int isnop_count;
}Data;

void print_nop(int* nop, int cycle_count){
    int counter = 0;
    for (int j = 0; j < 9; j++){
        if (counter >= 5){
            printf("%c", '.');
        }
        else{
            if (nop[j] >= 0){
                counter++;
            }
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
void print_mip(int* mip, int cycle_count){
    for (int j = 0; j < 9; j++){
        if (mip[j] == -1){
            printf("%c", '.');
        }
        if (mip[j] == 0){
            printf("%s", "IF");
        }
        if (mip[j] == 1){
            printf("%s", "ID");
        }
        if (mip[j] == 2){
            printf("%s", "EX");
        }
        if (mip[j] == 3){
            printf("%s", "MEM");
        }
        if (mip[j] == 4){
            printf("%s", "WB");
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

    int linecount = 0;
    int cycle_count = 0;
    int i = 0;
    int j = 0;
    char lines[5][128];

    int nops1[9];
    int nops2[9];
    //Data* mips = calloc(5, sizeof(Data));   
    Data mips[5];
    char dependency[3];

    for (i = 0; i < 9; i++){
            nops1[i] = -1;
            nops2[i] = -1;
    }

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
        //if (cycle_count == 10) break;
        if (mips[linecount-1].count == 5) break;
        printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
        for (i = 0; i < linecount; i++){
            if (cycle_count == 1){
                printf("%s", lines[i]);
                if (i == 0){
                    printf("IF\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
                else{
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }
            
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
            }

            else if (cycle_count == 4) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\t.\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    if (mips[i].isnop == 1){
                        
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\tIF\tID\tEX\t.\t.\t.\t.\t.\n");
                        mips[i].count++;
                    }
                }
                else if (i == 2) {
                    printf("%s", lines[i]);
                    printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
            }
            else if (cycle_count == 5) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf("%s", lines[i]);
                    printf(".\tIF\tID\tEX\tMEM\t.\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    if (mips[i].isnop == 1){
                        printf("nop\t\t");
                        for (j = 0; j < mips[i].count; j++){
                            nops1[2+j] = j;
                            mips[i].instructions[2+j] = j;
                        }
                        for (j = mips[i].count+2; j < 9; j++){
                            nops1[j] = 5;
                        }
                        mips[i].instructions[cycle_count-1] = mips[i].instructions[cycle_count-2];
                        
                        print_nop(nops1, cycle_count);
                        printf("%s", lines[i]);
                        print_mip(mips[i].instructions, cycle_count);
                        //mips[i].count++;
                        mips[i].isnop_count++;
                    }
                    else{
                        printf("%s", lines[i]);
                        printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                        mips[i].count++;
                        
                    }
                    
                }
            } 
            //after cycle 5, first line will finish running
            else if (cycle_count == 6) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf("%s", lines[i]);
                    printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    if (mips[i].isnop == 1 && mips[i].isnop_count > 1){
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
            }

            else if (cycle_count == 7) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf("%s", lines[i]);
                    printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    if (mips[i].isnop == 1 && mips[i].isnop_count > 1){
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
            }

            else if (cycle_count == 8) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf("%s", lines[i]);
                    printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    if (mips[i].isnop == 1 && mips[i].isnop_count > 1){
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
            }

            else if (cycle_count == 9) {
                if (i == 0) {
                    printf("%s", lines[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf("%s", lines[i]);
                    printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    mips[i].count++;
                }
                else if (i == 2) {
                    if (mips[i].isnop == 1 && mips[i].isnop_count > 1){
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
            }

            else{
                printf("%s", lines[i]);
                printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
            }

        }
        printf("\n");
    }

    fclose(file);
    printf("END OF SIMULATION\n");
    return EXIT_SUCCESS;
}