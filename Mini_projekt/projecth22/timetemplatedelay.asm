  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	PUSH($a0)
	li	$a0,1000
	jal	delay
	nop
	POP($a0)
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

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
	# void delay( int ms ) /* Wait a number of milliseconds, specified by the parameter value. */ {
	# int i;
	# while( ms > 0 ) {
	# ms = ms – 1; /* Executing the following for loop should take 1 ms */
	# for( i = 0; i < 4711; i = i + 1 ) /* The constant 4711 must be easy to change! */ {
	# /* Do nothing. */ } }
	# }
	whiledelay: # a0 = ms
		slt $t0, $0, $a0 # if a0 > 0 continue
		beq $t0, $0, donedelay
		nop
		addi $a0, $a0, -1 # a0 = a0 - 1
		li $t2, 0 # iterating variable for for loop
		fordelay: # should take 1 ms
			slti $t1, $t2, 4711 # if t2 < 4711 continue
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
	jr $ra
	nop
