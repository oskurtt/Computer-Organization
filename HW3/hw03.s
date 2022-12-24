################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 16
  sw $s0, 0($sp) #N - rows
  sw $s1, 4($sp) #M - cols
  sw $s2, 8($sp) #array
  sw $ra, 12($sp) #ret address
  # get N
  li $v0, 5 #v0 will store N
  syscall
  move $s0, $v0 #move $v0 to $s0

  # get M 
  li $v0, 5 #v0 will store M
  syscall
  move $s1, $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move $a0, $s0
  move $a1, $s1
  jal mm_alloc
  add $s2, $v0, $zero
  # do nested loops to read in values
  mul $t0, $s0, $s1 # size = N x M 
  move $t1, $zero # i = 0

  Loop: bge $t1, $t0, Exit
        li $v0, 5 #get next data
        syscall
        move $t2, $v0 #store data in t2
        sll $t3, $t1, 2
        add $t3, $t3, $s2
        sw $t2, 0($t3) # array[offset] = t2 

        addi $t1, $t1, 1
        j Loop
  Exit:

  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
  move $v0, $s0
  move $v1, $s1

  # restore stack, ra, and any saved registers, if necessary
  lw $s0, 0($sp) 
  lw $s1, 4($sp) 
  sw $s2, 16($sp)
  lw $ra, 12($sp)
  addi $sp, $sp, 16
  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary

  # Allocate sufficient space
  mul $a0, $a0, $a1 #len of array (row x col)
  sll $a0, $a0, 2 #bytes to allocate 2^2 = 4
  li $v0, 9
  syscall
  # set return value

  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  # do nested loops to print out values
  sub $sp, $sp, 4
  sw $ra, 0($sp)
  move $t0, $zero #i = 0
  move $t1, $zero #j = 0
  move $t2, $zero #offset
  mul $t3, $a0, $a1 #size = N x M

  Loop1: beq $t0, $t3, Exit_Print # if i = 0, exit
         beq $t1, $a1, Loop2
         sll $t2, $t0, 2 # store offset t3 = i x 4
         add $t2, $t2, $a2 # get address
         lw $t4, 0($t2) # t4 = array[i]

         li $v0, 1 
         move $a0, $t4
         syscall

         li $v0, 4
         la $a0, tab
         syscall

         addi $t0, $t0, 1
         addi $t1, $t1, 1
         j Loop1
  Loop2: move $t1, $zero #j = 0
         li $v0, 4
         la $a0, newline
         syscall
         j Loop1

  Exit_Print:

  li $v0, 4
  la $a0, newline
  syscall
    
  # restore stack, ra, and any saved registers, if necessary
  lw  $ra, 0($sp)
  addi $sp, $sp, 4
  # return to main
  jr  $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4
  sw $ra, 0($sp)

  # set up and call mm_alloc for result matrix
  move $t0, $a0 # store a0 in temp variable
  move $t1, $a1 # store a1 in temp variable

  move $a1, $a3 # store a3 in a1
  jal mm_alloc

  move $a0, $t0 # swap back
  move $a1, $t1 # swap a1 back with temp variable

  move $t1, $a2  # t1 = matrix 1
  move $t2, $s5  # t2 = matrix 2

  move $t0, $v0 # t0 = result matrix


  
  # three nested loops for matrix-matrix multiplication
  move $t3, $zero # t3 = i
  move $t4, $zero # t4 = j
  move $t5, $zero # t5 = k

  

  m_loop1: bge $t3, $a0, m_exit #if i >= M1 rows -> exit
           j m_loop2

  m_loop2: bge $t4, $a3, l2 #if j >= M2 cols -> l2
           j m_loop3

  m_loop3: bge $t5, $a1, l3   #if k >= M1 cols -> l3
           
           lw $t6, 0($t1) # load mat1->data[i][k]
           lw $t7, 0($t2) # load mat2->data[k][j]

           mul $t8, $t6, $t7 # t8 = mat1->data[i][k] x mat2->data[k][j]
           lw $t6, 0($t0)    # load whatever is in $t0
           add $t6, $t6, $t8 # add whatever is in the array with $t8
           sw $t6, 0($t0)    # store result back into array

           addi $t1, $t1, 4  # increment matrix 1
           mul $t7, $a3, 4   # M2 Column x 4 to calculate offset
           add $t2, $t2, $t7 # M2 address + $t7 offset

           addi $t5, $t5, 1  #increment k
           j m_loop3


  l2: addi $t3, $t3, 1 # i += 1
      move $t4, $zero # j = 0
      move $t5, $zero # k = 0
      mul $t7, $a1, 4 # $t7 = M1 col x 4
      add $t1, $t1, $t7 #M1 address + $t7 offset
      move $t2, $s5 #reset M2 to original address
      j m_loop1

  l3: addi $t4, $t4, 1 # j += 1
      move $t5, $zero # k = 0
      mul $t7, $a1, 4 
      sub $t1, $t1, $t7
      addi $t0, $t0, 4 #increment result array 
      move $t2, $s5 #reset M2
      addi $t2, $t2, 4 #next element in M2

      j m_loop2

  m_exit:

  # set return value
  
  # restore stack, ra, and any saved registers, if necessary 
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  # return to main
  jr  $ra
