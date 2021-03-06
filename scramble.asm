.data 0x10000000
seed:    .word 0xbabefade # CHANGE THIS VALUE FOR PROJECT 1
prompt:  .stringz "Enter your initials: " # the prompt
ptr:     .word 0x0        # this will hold a pointer to allocated memory
	
.text 0x00400000
main:
    addi $v0, $zero, 9  # sbrk syscall code
    addi $a0, $zero, 4  # allocate 4 bytes
    syscall
    la $t0, ptr
    sw $v0, 0($t0)      # save the address

    la $a0, prompt      # addres of prompt
    addi $a1, $zero, 22 # length of prompt
    addi $v0, $zero, 4  # print_string syscall code
    syscall

    lw $a0, 0($t0)      # get the address 
    addi $a1, $zero, 4  # read 4 bytes from user
    addi $v0, $zero, 8  # read_string syscall code
    syscall


    la $t0, ptr
    lw $t1, 0($t0)      # get the pointer to the initials
    lbu $t9, 0($t1)
    add $t0, $zero, $t9
    lbu $t9, 1($t1)
    add $t0, $t0, $t9
    lbu $t9, 2($t1)
    add $t0, $t0, $t9   # sum the ascii values of initials
                        # to use as a loop counter

    beq $t0, $zero, done
loop: 
    la $t2, seed
    lw $t8, 0($t2)      # read the seed value

    andi $t7, $t8, 0x1
    slt $t6, $zero, $t7
    beq $t6, $zero, odd
    # even fall-through##
    srl $t3, $t8, 13    #
    andi $t3, $t3, 0x1  # these instructions manipulate 
    addi $t4, $zero, 1  # your seed value in an arbitrary, 
    xor $t3, $t3, $t4   # but predictable way
    sll $t3, $t3, 31    #
    srl $t8, $t8, 1     #
    or $t8, $t8, $t3    #
    #####################

    j out # skip the taken target code
odd:
    # odd taken-target ##
    srl $t3, $t8, 9     # 
    andi $t3, $t3, 0x1  # same as the fall-through, manipulate
    xor $t3, $t3, $zero # the seed value (but in a different 
    sll $t3, $t3, 31    # way than above)
    srl $t8, $t8, 1     #
    or $t8, $t8, $t3    #
    #####################

out: 
    sw $t8, 0($t2)      # save the manipulation

    addi $t0, $t0, -1   # decrement a loop counter
    beq $t0, $zero, done
    j loop

done:
    sw $t8, 0($t2)      # save the final value

    addi $a0, $t8, 0    # print the final value
    addi $v0, $zero, 1  # print_int syscall code
    syscall

    jr $ra              # return from main (end the simulation)

