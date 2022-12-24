#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
//struct
struct S_reg {
    char reg[3];
    char variable; 
    int val;
};
//check if variable is in list
int is_in_list(struct S_reg *reg_list, char var, int size){
    for (int i = 0; i < size+1; i++){
        if (reg_list[i].variable == var){
            return 1;
        }
    }
    return 0; 
}
//find index of the variable
int find_index(struct S_reg *reg_list, char var, int size){
    for (int i = 0; i < size+1; i++){
        if (reg_list[i].variable == var){
            return i;
        }
    }
    return 0; 
}
//find the powers of the num 
int find_powers(int num, int* arr){
    int size = 0, new_size = 0, temp, i, j;
    int * tmp_arr = malloc(50 * sizeof(int)); 
    num = abs(num);
    for (i = 0; num > 0; i++){
        temp = num % 2;
        arr[size++] = temp;
        num = num / 2;
    }
    if (num == 0){
        for (i = 0, j = 0; i < size; i++){
            if (arr[i] == 1){
                if (size-i-1 != 0){
                    new_size ++;
                    tmp_arr[j] = size - i - 1; 
                    j++;
                }
                
                
            }
        }
        memcpy(arr, tmp_arr, new_size * sizeof(int));
        free (tmp_arr); 
        return new_size;
    } 
    else {
        return 0;
    }
}
//return the pow of 2
int is_pow_two(int num) {
    int count = 0;
    num = abs(num);
    while (num != 1) {
        if (num % 2 != 0){
            return 0;
        }
        num = num / 2;
        count++;
    }
    return count;
}

int main(int argc, char* argv[]){
    char* file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    
    int i , j, k, operation_count, index, left_index, right_index;
    char line[128];
    char* equation = malloc(128 * sizeof(char));

    struct S_reg s_reg_list[128]; 
    struct S_reg t_reg_list[128]; 
    int s_reg_count = 0;
    int t_reg_count = 0;
    int loop_count = 0;

    //read file
    while (fgets(line, sizeof(line), file)) {
        i = j = operation_count = 0;
        //equation = line w/out spaces
        for (i = 0, j = 0; line[i] != '\0'; i++){
            if (!isspace(line[i]) && line[i] != ';'){
                equation[j] = line[i];
                j ++;
                if (line[i] == '=' || line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%'){
                    operation_count += 1; 
                }
            }
        }
        printf("# %s", line);

        /////////////////
        //             //
        // li function //
        //             //
        /////////////////

        //if operation count = 1 (or if there is a negative number to store), then print li
        if (operation_count == 1 || (equation[1] == '=' && equation[2] == '-')){
            //set r value
            char* r_val = malloc(strlen(equation) * sizeof(char)); 
            for (i = 2, j = 0; equation[i] != '\0'; i++){
                r_val[j] = equation[i];
                j ++; 
            }
            r_val[j] = '\0';
            //check to see if variable is already in our list
            if (is_in_list(s_reg_list, equation[0], s_reg_count) == 0){
                //covert s_reg_count to character
                char c = s_reg_count + '0';

                //set register and its variable
                s_reg_list[s_reg_count].reg[0] = 's';
                s_reg_list[s_reg_count].reg[1] = c;
                s_reg_list[s_reg_count].reg[2] = '\0';
                s_reg_list[s_reg_count].variable = equation[0]; 
                s_reg_list[s_reg_count].val = atoi(r_val);

                //print mips code
                printf("li $%s,%s\n", s_reg_list[s_reg_count].reg, r_val); 
                 
                s_reg_count ++;
            }
            else{
                index = find_index(s_reg_list, equation[0], s_reg_count);
                printf("li $%s,%s\n", s_reg_list[index].reg, r_val);
            }
            free (r_val);
        }

        //////////////////////////////////////////////////////
        //                                                  //
        //                                                  //
        //if operation_count = 2, no need to use t registers//
        //                                                  //
        //                                                  //
        //////////////////////////////////////////////////////
        else if (operation_count == 2){
            //check to see if variable is in list
            if (is_in_list(s_reg_list, equation[0], s_reg_count) == 0){
                //covert s_reg_count to character
                char c = s_reg_count + '0';

                //set register and its variable
                s_reg_list[s_reg_count].reg[0] = 's';
                s_reg_list[s_reg_count].reg[1] = c;
                s_reg_list[s_reg_count].reg[2] = '\0';
                s_reg_list[s_reg_count].variable = equation[0]; 
                s_reg_count ++;
            }

            index = find_index(s_reg_list, equation[0], s_reg_count);
            left_index = find_index(s_reg_list, equation[2], s_reg_count);
            
            //////////////////
            // op = 2 and + //
            //////////////////
            if (equation[3] == '+'){
                //checks if we are using add or addi
                //if right side of operand is alphabet, then we need t-registers
                //if right side of operand is number, then we reach the end of our equation
                if (isalpha(equation[4])){
                    printf("add ");
                    right_index = find_index(s_reg_list, equation[4], s_reg_count);
                    printf("$%s,$%s,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                }
                else{
                    printf("addi ");
                    char right_num [strlen(equation)];
                    for (i = 4, j=0; equation[i] != '\0'; i++){
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0';
                    printf("$%s,$%s,%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg, right_num);
                }
            }   

            //////////////////
            // op = 2 and - //
            //////////////////
            else if (equation[3] == '-'){
                if (isalpha(equation[4])){
                    printf("sub ");
                    right_index = find_index(s_reg_list, equation[4], s_reg_count);
                    printf("$%s,$%s,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                }
                else{
                    printf("addi ");
                    char right_num [strlen(equation)];
                    for (i = 3, j=0; equation[i] != '\0'; i++){
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0';
                    printf("$%s,$%s,%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg, right_num);
                }
            }

            //////////////////
            // op = 2 and * //
            //////////////////
            else if (equation[3] == '*'){
                if (isalpha(equation[4])){
                    printf("mult ");
                    right_index = find_index(s_reg_list, equation[4], s_reg_count);
                    printf("$%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mflo $%s\n", s_reg_list[index].reg);
                }
                else{
                    //only if multiplying by positive number 
                    char right_num [strlen(equation)];
                    for (i = 4, j=0; equation[i] != '\0'; i++){
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0';
                    int* arr = malloc(50 * sizeof(int));
                    int size = 0;
                    size = find_powers(atoi(right_num), arr); 


                    if (atoi(right_num) == 0){
                        printf("li $%s,0\n", s_reg_list[index].reg);
                    }
                    else if (size == 0){
                        printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                        printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);
                        t_reg_count ++;
                    }
                    else{
                        t_reg_count++;
                        for (i = 0; i < size; i++){
                            printf("sll $t%d,$%s,%d\n", t_reg_count-1, s_reg_list[left_index].reg, arr[i]);
                            if (i == 0){
                                printf("move $t%d,$t%d\n", t_reg_count, t_reg_count-1);
                            }
                            else if (i == size-1){
                                printf("add $t%d,$t%d,$t%d\n", t_reg_count,t_reg_count,t_reg_count-1);
                                printf("add $t%d,$t%d,$%s\n", t_reg_count, t_reg_count, s_reg_list[left_index].reg);
                                printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);
                            }
                            else{
                                printf("add $t%d,$t%d,$t%d\n", t_reg_count,t_reg_count,t_reg_count-1); 
                            }
                        }
                        t_reg_count ++; 
                    }
                    free(arr); 
                }
            }

            //////////////////
            // op = 2 and / //
            //////////////////
            else if (equation[3] == '/'){
                if (isalpha(equation[4])){
                    printf("div ");
                    right_index = find_index(s_reg_list, equation[4], s_reg_count);
                    printf("$%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mflo $%s\n", s_reg_list[index].reg);
                }
                else{
                    //only if dividing by positive number
                    char right_num [strlen(equation)];
                    for (i = 4, j=0; equation[i] != '\0'; i++){
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0'; 
                    if (atoi(right_num) == 1){
                        printf("move $%s,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                    }
                    else if (atoi(right_num) == -1){
                        printf("move $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                    }
                    else{
                        printf("bltz $%s,L%d\n", s_reg_list[left_index].reg, loop_count);
                        int pow = is_pow_two(atoi(right_num));
                        if (pow != 0){
                            printf("srl $%s,$%s,%d\n", s_reg_list[index].reg, s_reg_list[left_index].reg, pow);
                            printf("j L%d\n", loop_count + 1);
                        }
                        printf("L%d:\n", loop_count);
                        printf("li $t%d,%s\n", t_reg_count, right_num);
                        printf("div $%s,$t%d\n", s_reg_list[left_index].reg, t_reg_count);
                        printf("mflo $%s\n", s_reg_list[index].reg);
                        printf("L%d:\n", loop_count + 1);
                        t_reg_count ++;
                        loop_count += 2;
                    }
                    
                }
            }

            //////////////////
            // op = 2 and % //
            //////////////////
            else if (equation[3] == '%'){
                if (isalpha(equation[4])){
                    printf("div ");
                    right_index = find_index(s_reg_list, equation[4], s_reg_count);
                    printf("$%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mfhi $%s\n", s_reg_list[index].reg);
                }
                else{
                    //only if modding by positive number 










                }
            }
        }

        //////////////////////////////////////////////////////
        //                                                  //
        //                                                  //
        //if operation_count > 2                            //
        //                                                  //
        //                                                  //
        //////////////////////////////////////////////////////

        else if (operation_count > 2){
            if (is_in_list(s_reg_list, equation[0], s_reg_count) == 0){
                //covert s_reg_count to character
                char c = s_reg_count + '0';

                //set register and its variable
                s_reg_list[s_reg_count].reg[0] = 's';
                s_reg_list[s_reg_count].reg[1] = c;
                s_reg_list[s_reg_count].reg[2] = '\0';
                s_reg_list[s_reg_count].variable = equation[0]; 
                s_reg_count ++;
            }
            
            index = find_index(s_reg_list, equation[0], s_reg_count);
            left_index = find_index(s_reg_list, equation[2], s_reg_count);
            if (isalpha(equation[4])){
                right_index = find_index(s_reg_list, equation[4], s_reg_count);
            }
            char c = t_reg_count + '0';
            t_reg_list[t_reg_count].reg[0] = 't';
            t_reg_list[t_reg_count].reg[1] = c;
            t_reg_list[t_reg_count].reg[2] = '\0';
            
            if (equation[3] == '+'){
                if (isalpha(equation[4])){
                    printf("add $%s,$%s,$%s\n", t_reg_list[t_reg_count].reg, s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                }
                else{
                    char right_num [strlen(equation)];
                    for (i = 4, j=0; equation[i] != '\0'; i++){
                        if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                            break;
                        }
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0';
                    printf("add $%s,$%s,%s\n", t_reg_list[t_reg_count].reg, s_reg_list[left_index].reg, right_num);
                }
            }
            else if (equation[3] == '-'){
                if (isalpha(equation[4])){
                    printf("sub $%s,$%s,$%s\n", t_reg_list[t_reg_count].reg, s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                }
                else{
                    char right_num [strlen(equation)];
                    right_num[0] = '-';
                    for (i = 4, j=1; equation[i] != '\0'; i++){
                        if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                            break;
                        }
                        right_num[j] = equation[i];
                        j++;
                    }
                    right_num[j] = '\0';
                    printf("addi $%s,$%s,%s\n", t_reg_list[t_reg_count].reg, s_reg_list[left_index].reg, right_num);
                }
            }

            else if (equation[3] == '*'){
                if (isalpha(equation[4])){
                    printf("mult $%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mflo $%s\n", t_reg_list[t_reg_count].reg);
                }
                else{
                    char right_num [strlen(equation)];
                    if (equation[4] == '-'){
                        for (i = 4, j=0; equation[i] != '\0'; i++){
                            if (equation[i] == '+' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                                break;
                            }
                            right_num[j] = equation[i];
                            j++;
                        }   
                    }
                    else{
                        for (i = 4, j=0; equation[i] != '\0'; i++){
                            if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                                break;
                            }
                            right_num[j] = equation[i];
                            j++;
                        }
                    }
                    
                    right_num[j] = '\0';
                    int* arr = malloc(50 * sizeof(int));
                    int size = 0;
                    size = find_powers(atoi(right_num), arr); 
                    if (atoi(right_num) == 0){
                        printf("li $%s,0\n", s_reg_list[index].reg);
                    }
                    else if (size == 0){
                        if (atoi(right_num) == -1){
                            printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                            printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                            t_reg_count++;
                        }
                        else{
                            printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                            printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);  
                            t_reg_count++;
                        }
                    }
                    else{
                        t_reg_count ++;
                        for (i = 0; i < size; i++){
                            printf("sll $t%d,$%s,%d\n", t_reg_count-1, s_reg_list[left_index].reg, arr[i]);
                            if (i == 0){
                                printf("move $t%d,$t%d\n",t_reg_count,t_reg_count-1);
                            }
                            else if (i == size-1){
                                printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1);
                                printf("add $t%d,$t%d,$%s\n", t_reg_count, t_reg_count, s_reg_list[left_index].reg);
                                if (atoi(right_num) < 0){
                                    printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                }
                                else{
                                    printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                }
                                
                            }
                            else{
                                printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1); 
                            }
                        }
                        t_reg_count ++; 
                    }
                    free(arr);    
                }
            }

            else if (equation[3] == '/'){
                if (isalpha(equation[4])){
                    printf("div $%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mflo $%s\n", t_reg_list[t_reg_count].reg);
                }
                else {
                    char right_num [strlen(equation)];
                    if (equation[4] == '-'){
                        for (i = 4, j=0; equation[i] != '\0'; i++){
                            if (equation[i] == '+' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                                break;
                            }
                            right_num[j] = equation[i];
                            j++;
                        }   
                    }
                    else{
                        for (i = 4, j=0; equation[i] != '\0'; i++){
                            if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                                break;
                            }
                            right_num[j] = equation[i];
                            j++;
                        }
                    }
                    
                    right_num[j] = '\0';

                    if (atoi(right_num) == 1){
                        printf("move $t%d,$%s\n", t_reg_count-1, s_reg_list[left_index].reg);
                    }
                    else if (atoi(right_num) == -1){
                        if (operation_count <= 3){
                            printf("sub $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                        }
                        else{
                            printf("move $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                        }
                        
                    }
                    else{
                        if (operation_count > 3){
                            printf("li $t%d,%s\n", t_reg_count, right_num);
                            printf("div $%s,$t%d\n", s_reg_list[left_index].reg, t_reg_count);
                            t_reg_count ++;
                            printf("mflo $t%d\n", t_reg_count);
                        }
                        else{
                            printf("bltz $%s,L%d\n", s_reg_list[left_index].reg, loop_count);
                            int pow = is_pow_two(atoi(right_num));
                            if (pow != 0){
                                printf("srl $%s,$%s,%d\n", s_reg_list[index].reg, s_reg_list[left_index].reg, pow);
                                if (right_num[0] == '-'){
                                    printf("sub $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[index].reg);
                                }
                                printf("j L%d\n", loop_count + 1);
                            }
                            printf("L%d:\n", loop_count);
                            printf("li $t%d,%s\n", t_reg_count, right_num);
                            printf("div $%s,$t%d\n", s_reg_list[left_index].reg, t_reg_count);
                            printf("mflo $%s\n", s_reg_list[index].reg);
                            printf("L%d:\n", loop_count + 1);
                            t_reg_count ++;
                            loop_count += 2;
                        }
                        
                    }

                    /*printf("li $t%d,%s\n", t_reg_count, right_num);
                    printf("div $%s,$t%d\n", s_reg_list[left_index].reg, t_reg_count);
                    t_reg_count++;
                    printf("mflo $t%d\n", t_reg_count);*/
                }
            }

            else if (equation[3] == '%'){
                if (isalpha(equation[4])){
                    printf("div $%s,$%s\n", s_reg_list[left_index].reg, s_reg_list[right_index].reg);
                    printf("mfhi $%s\n", t_reg_list[t_reg_count].reg);
                }
            }
            t_reg_count ++;
            
            //check if we are at end of equation
            int end_of_equation = -1; 
            for (i = 5; equation[i] != '\0'; i++){
                if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%'){
                    for (j = i+2; equation[j] != '\0'; j++){
                        if (equation[j] == '+' || equation[j] == '-' || equation[j] == '*' || equation[j] == '/' || equation[j] == '%'){
                            break;
                        }
                    }
                    if (equation[j] == '\0'){
                        end_of_equation = 1;
                    }
                }
                /////////////////
                //op > 2 and + //
                /////////////////
                if (equation[i] == '+'){
                    if (end_of_equation == 1){
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            for (k = i+1, j=0; equation[k] != '\0'; k++){
                                right_num[j] = equation[k];
                                j++;
                            }
                            right_num[j] = '\0';
                            printf("addi $%s,$%s,%s\n", s_reg_list[index].reg, t_reg_list[t_reg_count-1].reg, right_num);
                        }
                        else{
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("add $%s,$%s,%s\n", s_reg_list[index].reg, t_reg_list[t_reg_count-1].reg, s_reg_list[right_index].reg);
                        }
                    }
                    else{
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            for (k = i+1, j=0; equation[k] != '\0'; k++){
                                if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                    break;
                                }
                                right_num[j] = equation[k];
                                j++;
                            }
                            right_num[j] = '\0';

                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;


                            printf("addi $%s,$%s,%s\n", t_reg_list[t_reg_count-1].reg, t_reg_list[t_reg_count-2].reg, right_num);

                        }
                        else{
                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("add $%s,$%s,$%s\n", t_reg_list[t_reg_count-1].reg, t_reg_list[t_reg_count-2].reg, s_reg_list[right_index].reg);
                        }
                    }
                }

                /////////////////
                //op > 2 and - //
                /////////////////
                else if (equation[i] == '-'){
                    if (end_of_equation == 1){
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            for (k = i, j=0; equation[k] != '\0'; k++){
                                right_num[j] = equation[k];
                                j++;
                            }
                            right_num[j] = '\0';
                            printf("addi $%s,$%s,%s\n", s_reg_list[index].reg, t_reg_list[t_reg_count-1].reg, right_num);
                        }
                        else{
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("sub $%s,$%s,$%s\n", s_reg_list[index].reg, t_reg_list[t_reg_count-1].reg, s_reg_list[right_index].reg);
                        }
                    }
                    else{
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            right_num[0] = '-';
                            for (k = i+1, j=1; equation[k] != '\0'; k++){
                                if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                    break;
                                }
                                right_num[j] = equation[k];
                                j++;
                            }
                            right_num[j] = '\0';

                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;


                            printf("addi $%s,$%s,%s\n", t_reg_list[t_reg_count-1].reg, t_reg_list[t_reg_count-2].reg, right_num);

                        }
                        else{
                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("sub $%s,$%s,$%s\n", t_reg_list[t_reg_count-1].reg, t_reg_list[t_reg_count-2].reg, s_reg_list[right_index].reg);
                        }
                    }
                }

                /////////////////
                //op > 2 and * //
                /////////////////
                else if (equation[i] == '*'){
                    if (equation[i+1] == '0'){
                        printf("li $%s,0\n", s_reg_list[index].reg);
                        break;
                    }
                    if (end_of_equation == 1){
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            if (equation[i+1] == '-'){
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }   
                            }
                            else{
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }
                            }
                            right_num[j] = '\0';
                            int* arr = malloc(50 * sizeof(int));
                            int size = 0;
                            size = find_powers(atoi(right_num), arr); 
                            if (atoi(right_num) == 0){
                                printf("li $%s,0\n", s_reg_list[index].reg);
                            }
                            else if (size == 0){
                                if (atoi(right_num) == -1){
                                    printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                                    printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                    t_reg_count++;
                                }
                                else{
                                    printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                                    printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);  
                                    t_reg_count++;
                                }
                            }
                            else{
                                t_reg_count ++;
                                for (i = 0; i < size; i++){
                                    printf("sll $t%d,$%s,%d\n", t_reg_count-1, s_reg_list[left_index].reg, arr[i]);
                                    if (i == 0){
                                        printf("move $t%d,$t%d\n",t_reg_count,t_reg_count-1);
                                    }
                                    else if (i == size-1){
                                        printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1);
                                        printf("add $t%d,$t%d,$%s\n", t_reg_count, t_reg_count, s_reg_list[left_index].reg);
                                        if (atoi(right_num) < 0){
                                            printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                        }
                                        else{
                                            printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                        }
                                        
                                    }
                                    else{
                                        printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1); 
                                    }
                                }
                                t_reg_count ++; 
                            }
                            free(arr);    
                        }
                        else{
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("mult $t%d,$%s\n", t_reg_count-2, s_reg_list[right_index].reg);
                            printf("mflo $%s\n", s_reg_list[index].reg);
                        }
                    }
                    else{
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            if (equation[i+1] == '-'){
                                for (k = i + 1, j=0; equation[k] != '\0'; i++){
                                    if (equation[k] == '+' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }   
                            }
                            else{
                                for (k = i + 1, j=0; equation[i] != '\0'; i++){
                                    if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }
                            }
                            right_num[j] = '\0';
                            int* arr = malloc(50 * sizeof(int));
                            int size = 0;
                            size = find_powers(atoi(right_num), arr); 
                            if (atoi(right_num) == 0){
                                printf("li $%s,0\n", s_reg_list[index].reg);
                            }
                            else if (size == 0){
                                if (atoi(right_num) == -1){
                                    printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                                    printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                    t_reg_count++;
                                }
                                else{
                                    printf("move $t%d,$%s\n", t_reg_count, s_reg_list[left_index].reg);
                                    printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);  
                                    t_reg_count++;
                                }
                            }
                            else{
                                t_reg_count ++;
                                for (i = 0; i < size; i++){
                                    printf("sll $t%d,$%s,%d\n", t_reg_count-1, s_reg_list[left_index].reg, arr[i]);
                                    if (i == 0){
                                        printf("move $t%d,$t%d\n",t_reg_count,t_reg_count-1);
                                    }
                                    else if (i == size-1){
                                        printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1);
                                        printf("add $t%d,$t%d,$%s\n", t_reg_count, t_reg_count, s_reg_list[left_index].reg);
                                        if (atoi(right_num) < 0){
                                            printf("sub $%s,$zero,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                        }
                                        else{
                                            printf("move $%s,$t%d\n", s_reg_list[index].reg, t_reg_count);
                                        }
                                        
                                    }
                                    else{
                                        printf("add $t%d,$t%d,$t%d\n", t_reg_count, t_reg_count, t_reg_count-1); 
                                    }
                                }
                                t_reg_count ++; 
                            }
                            free(arr);    

                        }
                        else{
                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("mult $t%d,$%s\n", t_reg_count-2, s_reg_list[right_index].reg);
                            printf("mflo $t%d\n", t_reg_count-1);
                        }
                    }
                }

                /////////////////
                //op > 2 and / //
                /////////////////
                else if (equation[i] == '/'){
                    if (end_of_equation == 1){
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            if (equation[i+1] == '-'){
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }   
                            }
                            else{
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }
                            }
                            right_num[j] = '\0'; 
                            if (atoi(right_num) == 1){
                                printf("move $%s,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                            }
                            else if (atoi(right_num) == -1){
                                printf("move $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                            }
                            else{
                                printf("bltz $t%d,L%d\n", t_reg_count-1, loop_count);
                                int pow = is_pow_two(atoi(right_num));
                                if (pow != 0){
                                    printf("srl $%s,$t%d,%d\n", s_reg_list[index].reg, t_reg_count-1, pow);
                                    //printf("sub $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                                    printf("j L%d\n", loop_count + 1);
                                }
                                printf("L%d:\n", loop_count);
                                printf("li $t%d,%s\n", t_reg_count, right_num);
                                printf("div $t%d,$t%d\n", t_reg_count-1, t_reg_count);
                                printf("mflo $%s\n", s_reg_list[index].reg);
                                printf("L%d:\n", loop_count + 1);
                                t_reg_count ++;
                                loop_count += 2;
                            }
                        }
                        else{
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("div $%s,$%s\n", t_reg_list[t_reg_count-1].reg, s_reg_list[right_index].reg);
                            printf("mflo $%s\n", s_reg_list[index].reg);
                        }
                    }
                    else{
                        if (!isalpha(equation[i+1])){
                            char right_num [strlen(equation)];
                            if (equation[i+1] == '-'){
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }   
                            }
                            else{
                                for (k = i + 1, j=0; equation[k] != '\0'; k++){
                                    if (equation[k] == '+' || equation[k] == '-' || equation[k] == '*' || equation[k] == '/' || equation[k] == '%'){
                                        break;
                                    }
                                    right_num[j] = equation[k];
                                    j++;
                                }
                            }
                            right_num[j] = '\0'; 
                            if (atoi(right_num) == 1){
                                printf("move $%s,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                            }
                            else if (atoi(right_num) == -1){
                                printf("move $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                            }
                            else{
                                printf("bltz $t%d,L%d\n", t_reg_count-1, loop_count);
                                int pow = is_pow_two(atoi(right_num));
                                if (pow != 0){
                                    printf("srl $%s,$%s,%d\n", s_reg_list[index].reg, s_reg_list[left_index].reg, pow);
                                    printf("sub $%s,$zero,$%s\n", s_reg_list[index].reg, s_reg_list[left_index].reg);
                                    printf("j L%d\n", loop_count + 1);
                                }
                                printf("L%d:\n", loop_count);
                                printf("li $t%d,%s\n", t_reg_count, right_num);
                                printf("div $%s,$t%d\n", s_reg_list[left_index].reg, t_reg_count);
                                printf("mflo $%s\n", s_reg_list[index].reg);
                                printf("L%d:\n", loop_count + 1);
                                t_reg_count ++;
                                loop_count += 2;
                            }
                        }
                        else{
                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("div $%s,$%s\n", t_reg_list[t_reg_count-2].reg, s_reg_list[right_index].reg);
                            printf("mflo $%s\n", t_reg_list[t_reg_count-1].reg);
                        }
                    }
                }

                /////////////////
                //op > 2 and % //
                /////////////////
                else if (equation[i] == '%'){
                    if (end_of_equation == 1){
                        if (!isalpha(equation[i+1])){









                        }
                        else{
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("div $%s,$%s\n", t_reg_list[t_reg_count-1].reg, s_reg_list[right_index].reg);
                            printf("mfhi $%s\n", s_reg_list[index].reg);
                        }
                    }
                    else{
                        if (!isalpha(equation[i+1])){











                        }
                        else{
                            char c = t_reg_count + '0';
                            t_reg_list[t_reg_count].reg[0] = 't';
                            t_reg_list[t_reg_count].reg[1] = c;
                            t_reg_list[t_reg_count].reg[2] = '\0';
                            t_reg_count ++;
                            right_index = find_index(s_reg_list, equation[i+1], s_reg_count);
                            printf("div $%s,$%s\n", t_reg_list[t_reg_count-2].reg, s_reg_list[right_index].reg);
                            printf("mfhi $%s\n", t_reg_list[t_reg_count-1].reg);
                        }
                    }
                }
            }
        }



        ////////////////////
        //                //
        // empty equation //
        //                //
        ////////////////////
        for (i = 0; equation[i] != '\0'; i++){
            equation[i] = '\0';
        }
    }
    return 0;
}