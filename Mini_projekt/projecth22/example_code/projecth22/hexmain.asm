  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,15		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #

hexasc:
	andi $a0,$a0,0x0000000F # mask bits so only the lower 4 are used
	caselowerthan10hexasc:
		slti $t0, $a0, 10 # if a0<10 continue
		beq $t0, $0, casehigherthan9hexasc
		nop
		addi $v0, $a0, 0x30 # adds the number to the ASCII-value of 0
		j donehexasc
		nop # delay slot filler (just in case)
	casehigherthan9hexasc:
		addi $t0, $0, 9
		slt $t1, $t0, $a0 # if a0>9 continue
		beq $t1, $0, donehexasc
		nop
		addi $t2, $a0, -10
		addi $v0, $t2, 0x41 #  adds the number minus 10 to the ASCII-value of 0
		j donehexasc
		nop # delay slot filler (just in case)
	donehexasc:
	
	jr $ra # goes back to main
	nop # delay slot filler (just in case)

