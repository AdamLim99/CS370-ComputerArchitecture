.data 0x10000000
.byte 222
.byte 250
.byte 190
.byte 186
.byte 69
.byte 110
.byte 116
.byte 101
.byte 114
.byte 32
.byte 121
.byte 111
.byte 117
.byte 114
.byte 32
.byte 105
.byte 110
.byte 105
.byte 116
.byte 105
.byte 97
.byte 108
.byte 115
.byte 58
.byte 32
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.text 0x00400000
addi $v0, $zero, 9
addi $a0, $zero, 4
syscall
lui $t0, 0x1000
ori $t0, $t0, 0x001c
sw $v0, 0($t0)
lui $a0, 0x1000
ori $a0, $a0, 0x0004
addi $a1, $zero, 22
addi $v0, $zero, 4
syscall
lw $a0, 0($t0)
addi $a1, $zero, 4
addi $v0, $zero, 8
syscall
lui $t0, 0x1000
ori $t0, $t0, 0x001c
lw $t1, 0($t0)
lbu $t9, 0($t1)
add $t0, $zero, $t9
lbu $t9, 1($t1)
add $t0, $t0, $t9
lbu $t9, 2($t1)
add $t0, $t0, $t9
beq $t0, $zero, 0x00400004
lui $t2, 0x1000
ori $t2, $t2, 0x0000
lw $t8, 0($t2)
andi $t7, $t8, 0x0001
slt $t6, $zero, $t7
beq $t6, $zero, 0x0043ffac
srl $t3, $t8, 13
andi $t3, $t3, 0x0001
addi $t4, $zero, 1
xor $t3, $t3, $t4
sll $t3, $t3, 31
srl $t8, $t8, 1
or $t8, $t8, $t3
j 0x004000b4
srl $t3, $t8, 9
andi $t3, $t3, 0x0001
xor $t3, $t3, $zero
sll $t3, $t3, 31
srl $t8, $t8, 1
or $t8, $t8, $t3
sw $t8, 0($t2)
addi $t0, $t0, -1
beq $t0, $zero, 0x0043ff4c
j 0x00400064
sw $t8, 0($t2)
addi $a0, $t8, 0
addi $v0, $zero, 1
syscall
jr $ra
