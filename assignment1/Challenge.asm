.global main
.data
select: .asciiz "Select Procedure : "
ais: .asciiz "a is : "
bis: .asciiz "b is : "
nis: .asciiz "n is : "
ismult: .asciiz " is multiple of three!"
isnmult: .asciiz " is not multiple of three!"
initstate: .asciiz "1   1   "
stab: .asciiz "   "

.text
main:
la $a0 select
li $v0 4
syscall # print_string "Select Procedure : "
li $v0 5
syscall # read_int placed in $v0

slti $t0 $v0 2 # $t0 = ($v0 < 2)
beq $t0 $zero L2 # if select procedure is 2 goto L

L1:
la $a0 ais
li $v0 4
syscall # print_string "a is : "
li $v0 5
syscall # read_int placed in $v0
add $s0 $v0 $zero # $s0 = a
la $a0 bis
li $v0 4
syscall # print_string "b is : "
li $v0 5
syscall # read_int placed in $v0
add $s1 $v0 $zero # set $a1 by input value of b
sub $a0 $s0 $s1 # $a0 = a - b
jal isMultofThree # goto isMultofThree
j Exit

L2: # select procedure 2
la $a0 nis
li $v0 4
syscall # print_string "n is : "
li $v0 5
syscall # read_int placed in $v0
add $s0 $v0 $zero # $s0 = n
la $a0 ais
li $v0 4
syscall # print_string "a is : "
li $v0 5
syscall # read_int placed in $v0
add $s1 $v0 $zero # $s1 = a
add $a0 $s0 $zero # $a0 = n
add $a1 $s1 $zero # $a1 = a
jal myFibo # goto myFibo
j Exit

Exit:
li $v0 10
syscall # program end

isMultofThree: # $a0 = a - b
addi $t0 $t0 3
# $t0 = 3
div $a0 $t0
mfhi $t0
# $t0 = (a - b) % 3
bne $t0 $zero Ln
li $v0 1
syscall
# print_int $a0 = a - b
la $a0 ismult
li $v0 4
syscall
jr $ra
# print_string " is multiple of three!"
Ln: # not multiple of three
li $v0 1
syscall
# print_int $a0 = a - b
la $a0 isnmult
li $v0 4
syscall
# print_string " is not multiple of three!"
jr $ra

myFibo:
addi $sp $sp -4
sw $a0 0($sp) # save $a0
add $s0 $a0 $zero # $s0 = n
addi $s1 $zero 1 # $s1 = i = 1 (i < n) do-while
addi $t0 $t0 1 # $t0 init (0, 1)
addi $t1 $t1 1 # $t1 init (1, 1)
la $a0 initstate
li $v0 4
syscall # print_string "1   1   "

Loop:
add $t0 $t0 $t1
add $t0 $t0 $a1 # $t0 = $t0 + $t1 + a
add $a0 $t0 $zero # $a0 = fibo(i + 1)
li $v0 1 # print_int fibo(i + 1)
syscall
la $a0 stab
li $v0 4
syscall # print_int "   "

add $t2 $t1 $zero
add $t1 $t0 $zero
add $t0 $t2 $zero # swap $t0 $t1

addi $s1 $s1 1
slt $t2 $s1 $s0 # if $t2 == 1 loop
bne $t2 $zero Loop
lw $a0 0($sp) # load $a0
addi $sp $sp 4
jr $ra






# recursive procedure (need to fix)
# need array not print every callee
# after end of all procedure, print
myFibo:
addi $sp $sp -8
sw $ra 4($sp) # save $ra
sw $a0 0($sp) # save $a0

slti $t0 $a0 2 # $t0 = $a0 < 2
beq $t0 $zero Recursive

Base: # n < 2
addi $a0 $zero 1 # $a0 = 1
li $v0 1
syscall # print_int n
la $a0 stab
li $v0 4
syscall # print_string "   "
lw $a0 0($sp)
lw $ra 4($sp)
addi $sp $sp 8
addi $v0 $zero 1
jr $ra

Recursive:
addi $a0 $a0 -2 # n - 2
jal myFibo # returned $v0 and $a0 = n - 2
add $v1 $v0 $zero
addi $a0 $a0 1 # n - 1
jal myFibo # returned $v0 and $a0 = n - 1
add $v1 $v1 $v0
add $v1 $v1 $a1 # $v1 = myFibo(n - 2) + myFibo(n - 1) + a
add $a0 $v1 $zero # $a0 = $v1
li $v0 1
syscall # print_int myFibo(n - 2) + myFibo(n - 1) + a
la $a0 stab
li $v0 4
syscall # print_string "   "
lw $a0 0($sp)
lw $ra 4($sp)
addi $sp $sp 8
add $v0 $v1 $zero # return $v0'
jr $ra
