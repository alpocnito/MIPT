	.p2align 4,,15
	.globl	_Z13HashTablePushP9HashTablePcPFyPvmE
	.type	_Z13HashTablePushP9HashTablePcPFyPvmE, @function
_Z13HashTablePushP9HashTablePcPFyPvmE:
.LFB93:
	pushq	%r12
	pushq	%rbp
    pushq	%rbx
	
    movq	%rdi, %rbp
	movq	%rsi, %rdi
	movq	%rsi, %rbx
	movq	%rdx, %r12
	call	strlen@PLT
	
    movq	%rbx, %rdi
	movq	%rax, %rsi
	call	*%r12
	
    movabsq	$-1712817109330099001, %rdx
	movq	%rax, %rcx
	movq	%rbx, %rsi
	mulq	%rdx
	movq	(%rbp), %rax
	shrq	$14, %rdx
	imulq	$18061, %rdx, %rdx
	subq	%rdx, %rcx
	movq	(%rax,%rcx,8), %rdi
	call	_Z8ListPushP4ListPc@PLT
	
   	popq	%rbx
	popq	%rbp
	popq	%r12

    movl $1, %eax
	ret
	.size	_Z13HashTablePushP9HashTablePcPFyPvmE, .-_Z13HashTablePushP9HashTablePcPFyPvmE

