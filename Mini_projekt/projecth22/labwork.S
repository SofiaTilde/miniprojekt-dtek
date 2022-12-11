  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
timstr: .data
.global delay, time2string
.text

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
	
delay:
	whiledelay: # a0 = ms
		slt $t0, $0, $a0 # if a0 > 0 continue
		beq $t0, $0, donedelay
		nop
		addi $a0, $a0, -1 # a0 = a0 - 1
		li $t2, 0 # iterating variable for for loop
		fordelay: # should take 1 ms
			slti $t1, $t2, 4000# if t2 < 110 continue
			beq $t1, $0, donefordelay
			nop
			addi $t2, $t2, 1 # increase $t2 by one
			j fordelay
			nop
		donefordelay:
	j whiledelay
	nop
	donedelay:
	jr $ra
	nop
 
 time2string:
 	PUSH $ra
 	
 	la $s0, ($a0)
 	add $s1, $a1, $0
 	
 	# minute x0:00
 	sra $a0,$a1,12 # shifts 12 bits
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0, 0($s0) # stores hexasc results
 	
 	# minute 0x:00 
 	sra $a0,$a1,8 # shifts 8 bits (ignore previous 4 bits, they are masked in hexasc)
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0, 1($s0) # store hexasc results in next memory slot
 	
 	# add colon
	add $t0,$0,0x3a # writes colon to register
	sb $t0, 2($s0)
 	
 	# second 00:x0
  	sra $a0,$a1,4 # shifts 4 bits (ignore previous 8 bits, they are masked in hexasc)
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0, 3($s0)
 	
 	# second 00:0x
  	sra $a0,$a1,0
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0, 4($s0)	
 	
 	# end of string
	add $t0,$0,0x00
	sb $t0, 5($s0)
	
 	POP $ra
 	jr $ra 
 	nop 	# delay slot filler (just in case)
 	
enable_interrupt:
	ei
	jr $ra
	nop
	