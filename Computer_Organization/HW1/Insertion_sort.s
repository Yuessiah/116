.data
Array: .word 9, 2, 8, 1, 6, 5, 4, 10, 3, 7 # you can change the element of array

.text
main:
	addi $t0, $zero, 4097      # $t0 = 0x00001001
	sll  $t0, $t0, 16          # set the base address of your array into $t0 = 0x10010000    

	and $s1, $s1, $zero #i
for:
	beq $s1, 10, exit
	addi $s1, $s1, 1

	la $s2, ($s1) #j
	while:
		beq $s2, $zero, for # j > 0 then goto for

		la $t1, ($t0)
		addi $a0, $zero, 4
		mul $s3, $s2, $a0
		add $t1, $t1, $s3
		lw $t2, ($t1) #A[j]
		addi $t9, $t1, -4
		lw $t3, ($t9) #A[j-1]
		slt $t7, $t2, $t3
		beq $t7, $zero, for

		j swap # swap
		swap_finished:
		addi $s2, $s2, -1 # j -= 1
	j while
j for

swap:
	sw $t2, ($t9)
	sw $t3, ($t1)
j swap_finished

exit:
	li   $v0, 10               # program stop
	syscall
