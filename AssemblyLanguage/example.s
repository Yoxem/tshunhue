global    main

section   .text
main:
	push rbp
	mov     rbp, rsp
	mov rax,0x1
	mov [rbp-8],rax
	mov rax,0x5
	mov [rbp-16],rax
	add rax,[rbp-8]
	mov rsp,rbp
	pop rbp
	ret