.section	.rodata
.LC0:
	.string	"list_pointers.cpp"
.text
.globl ListValid
.type _ListValid, function
ListValid:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L2
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$59, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L2:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L4
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	je	.L5
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$63, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L5:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L6
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$64, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L6:
	movl	$1, %eax
	jmp	.L3
.L4:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movl	$0, -12(%rbp)
.L9:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	subq	$1, %rax
	cmpq	%rax, %rdx
	jnb	.L7
	cmpq	$0, -8(%rbp)
	jne	.L8
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$72, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L8:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
	addl	$1, -12(%rbp)
	jmp	.L9
.L7:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L10
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L11
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$79, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L11:
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L10
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$80, %edx
	leaq	0, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L10:
	movl	$1, %eax
.L3:
	leave
	ret
	.size	ListValid, .-ListValid
