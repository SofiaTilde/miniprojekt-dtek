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
	li	$a0,2
	jal	delay
	nop
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

  # you can write your code for subroutine "hexasc" below this line
  #
  
  hexasc:
	andi $a0,$a0,0x0000000F # mask bits
	caselowerthan10:
		slti $t0, $a0, 10 # if a0<10 continue
		beq $t0, $0, casehigherthan9
		addi $v0, $a0, 0x30 # adds the number to the ASCII-value of 0
		j done
		nop # delay slot filler (just in case)
	casehigherthan9:
		addi $t0, $0, 9
		slt $t1, $t0, $a0 # if a0>=10 continue
		beq $t1, $0, done
		addi $t2, $a0, -10
		addi $v0, $t2, 0x41 #  adds the number minus 10 to the ASCII-value of 0
		j done
		nop # delay slot filler (just in case)
	done:
	
	jr $ra # goes back to main
	nop # delay slot filler (just in case)
	
delay:
 jr $ra
 nop
 
 time2string:
 	PUSH $a2
 	PUSH $ra
 	
 	# minute x0:00
 	sra $a0,$a1,12 # shifts 12 bits
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0,timstr # stores hexasc results
 	
 	# minute 0x:00 
 	sra $a0,$a1,8 # shifts 8 bits (ignore previous 4 bits, they are masked in hexasc)
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0,timstr+1 # store hexasc results in next memory slot
 	
 	# add colon
	add $a2,$0,0x3a # writes colon to register
	sb $a2, timstr+2
 	
 	# second 00:x0
  	sra $a0,$a1,4 # shifts 4 bits (ignore previous 8 bits, they are masked in hexasc)
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	#la	$a0,timstr
 	sb $v0,timstr+3
 	
 	# second 00:0x
  	sra $a0,$a1,0
 	jal hexasc 	
 	nop 	# delay slot filler (just in case)
 	sb $v0,timstr+4	
 	
 	# end of string
	add $a2,$0,0x00
	sb $a2, timstr+5
	
 	POP $ra
 	POP $a2
 	jr $ra 
 	nop 	# delay slot filler (just in case)
 	
