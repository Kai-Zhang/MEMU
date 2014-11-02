.global start
start:
	# Set up a stack for C code
	movl $0, %ebp
	movl $(128 << 20), %esp

	# Jump to C code for remaining initializations, and should ever return
	jmp init
