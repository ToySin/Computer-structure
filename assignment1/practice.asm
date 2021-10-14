.global main
.data
one: .asciiz "First Number : "
two: .asciiz "Second Number : "
msg: .asciiz "Sum : "

.text
main:
la $a0 one
li $v0 4
syscall

li $v0 5
syscall

add $t0 $v0 $zero

la $a0 two
li $v0 4
syscall

li $v0 5
syscall

add $t1 $v0 $zero

la $a0 msg
li $v0 4
syscall

add $a0 $t0 $t1

li $v0 1
syscall

li $v0 10
syscall
