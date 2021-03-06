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
beq $t0 $zero L2 # if select procedure is 2 goto L2

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
add $s1 $v0 $zero # $s1 = b
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
addi $t0 $zero 3 # $t0 = 3
div $a0 $t0
mfhi $t0 # $t0 = (a - b) % 3
bne $t0 $zero Ln # if $t0 % 3 != 0 goto Ln
li $v0 1
syscall # print_int $a0 = a - b
la $a0 ismult
li $v0 4
syscall # print_string " is multiple of three!"
jr $ra # return

Ln: # not multiple of three
li $v0 1
syscall # print_int $a0 = a - b
la $a0 isnmult
li $v0 4
syscall # print_string " is not multiple of three!"
jr $ra # return


myFibo:
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
jr $ra