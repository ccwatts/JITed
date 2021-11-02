	.file	"2.ll"
	.text
	.globl	changeX
	.align	16, 0x90
	.type	changeX,@function
changeX:                                # @changeX
	.cfi_startproc
# BB#0:                                 # %LU1
	movl	%edi, %eax
	ret
.Ltmp0:
	.size	changeX, .Ltmp0-changeX
	.cfi_endproc

	.type	x,@object               # @x
	.comm	x,4,4

	.section	".note.GNU-stack","",@progbits
