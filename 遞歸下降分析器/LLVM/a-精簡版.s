	.text
	.file	"a-\347\262\276\347\260\241\347\211\210.ll"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	movl	$0, -4(%rsp)
	movl	$12, -8(%rsp)
	movl	$48, -12(%rsp)
	movl	$48, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc


	.section	".note.GNU-stack","",@progbits
