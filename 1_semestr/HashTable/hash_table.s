	.file	"hash_table.cpp"
	.text
	.p2align 4,,15
	.globl	_Z9ListValidP4List
	.type	_Z9ListValidP4List, @function
_Z9ListValidP4List:
.LFB57:
	.cfi_startproc
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE57:
	.size	_Z9ListValidP4List, .-_Z9ListValidP4List
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"list_pointers.cpp"
.LC1:
	.string	"list != NULL"
	.text
	.p2align 4,,15
	.globl	_Z8ListInitP4List
	.type	_Z8ListInitP4List, @function
_Z8ListInitP4List:
.LFB58:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L8
	movq	$0, (%rdi)
	movq	$0, 16(%rdi)
	movq	$0, 8(%rdi)
	ret
.L8:
	leaq	_ZZ8ListInitP4ListE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$49, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE58:
	.size	_Z8ListInitP4List, .-_Z8ListInitP4List
	.p2align 4,,15
	.globl	_Z11ListDisinitP4List
	.type	_Z11ListDisinitP4List, @function
_Z11ListDisinitP4List:
.LFB59:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE59:
	.size	_Z11ListDisinitP4List, .-_Z11ListDisinitP4List
	.p2align 4,,15
	.globl	_Z9LListPushP4ListPc
	.type	_Z9LListPushP4ListPc, @function
_Z9LListPushP4ListPc:
.LFB60:
	.cfi_startproc
	movslq	_ZZ9LListPushP4ListPcE12number_calls(%rip), %rax
	leal	1(%rax), %edx
	salq	$4, %rax
	movl	%edx, _ZZ9LListPushP4ListPcE12number_calls(%rip)
	leaq	memory(%rip), %rdx
	addq	%rdx, %rax
	movq	8(%rdi), %rdx
	movq	%rsi, (%rax)
	movq	$0, 8(%rax)
	testq	%rdx, %rdx
	je	.L13
	movq	16(%rdi), %rcx
	addq	$1, %rdx
	movq	%rax, 8(%rcx)
	movq	%rax, 16(%rdi)
	xorl	%eax, %eax
	movq	%rdx, 8(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L13:
	addq	$1, %rdx
	movq	%rax, 16(%rdi)
	movq	%rax, (%rdi)
	movq	%rdx, 8(%rdi)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE60:
	.size	_Z9LListPushP4ListPc, .-_Z9LListPushP4ListPc
	.p2align 4,,15
	.globl	_Z10ListDeleteP4Listi
	.type	_Z10ListDeleteP4Listi, @function
_Z10ListDeleteP4Listi:
.LFB61:
	.cfi_startproc
	movq	8(%rdi), %rdx
	movslq	%esi, %rcx
	movl	$1, %eax
	cmpq	%rdx, %rcx
	jnb	.L14
	movq	(%rdi), %rcx
	testl	%esi, %esi
	leaq	-1(%rdx), %r8
	movq	8(%rcx), %rdx
	je	.L21
	cmpl	$1, %esi
	jle	.L17
	movq	%rdx, %rcx
	movl	$1, %eax
	jmp	.L18
	.p2align 4,,10
	.p2align 3
.L22:
	movq	8(%rcx), %rcx
.L18:
	addl	$1, %eax
	movq	8(%rdx), %rdx
	cmpl	%eax, %esi
	jne	.L22
.L17:
	movq	8(%rdx), %rdx
	xorl	%eax, %eax
	testq	%rdx, %rdx
	movq	%rdx, 8(%rcx)
	movq	%r8, 8(%rdi)
	je	.L23
.L14:
	rep ret
	.p2align 4,,10
	.p2align 3
.L21:
	movq	%rdx, (%rdi)
	movq	%r8, 8(%rdi)
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L23:
	movq	%rcx, 16(%rdi)
	ret
	.cfi_endproc
.LFE61:
	.size	_Z10ListDeleteP4Listi, .-_Z10ListDeleteP4Listi
	.section	.rodata.str1.1
.LC2:
	.string	"%s,"
	.text
	.p2align 4,,15
	.globl	_Z9PrintListP4List
	.type	_Z9PrintListP4List, @function
_Z9PrintListP4List:
.LFB62:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	testq	%rdi, %rdi
	je	.L33
	movq	(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L26
	leaq	.LC2(%rip), %rbp
	.p2align 4,,10
	.p2align 3
.L27:
	movq	(%rbx), %rdx
	xorl	%eax, %eax
	movq	%rbp, %rsi
	movl	$1, %edi
	call	__printf_chk@PLT
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L27
.L26:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movl	$10, %edi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
.L33:
	.cfi_restore_state
	leaq	_ZZ9PrintListP4ListE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$130, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE62:
	.size	_Z9PrintListP4List, .-_Z9PrintListP4List
	.p2align 4,,15
	.globl	_Z14HashTableValidP9HashTable
	.type	_Z14HashTableValidP9HashTable, @function
_Z14HashTableValidP9HashTable:
.LFB105:
	.cfi_startproc
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE105:
	.size	_Z14HashTableValidP9HashTable, .-_Z14HashTableValidP9HashTable
	.section	.rodata.str1.1
.LC3:
	.string	"hash_table.cpp"
.LC4:
	.string	"htable != NULL"
	.text
	.p2align 4,,15
	.globl	_Z13HashTableInitP9HashTable
	.type	_Z13HashTableInitP9HashTable, @function
_Z13HashTableInitP9HashTable:
.LFB91:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	testq	%rdi, %rdi
	je	.L48
	movq	%rdi, %rbx
	movl	$8, %esi
	movl	$18061, %edi
	call	calloc@PLT
	testq	%rax, %rax
	movq	%rax, (%rbx)
	je	.L37
	movq	%rax, %rbx
	leaq	144488(%rax), %rbp
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L50:
	addq	$8, %rbx
	movq	$0, (%rax)
	movq	$0, 16(%rax)
	cmpq	%rbx, %rbp
	movq	$0, 8(%rax)
	je	.L49
.L39:
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	testq	%rax, %rax
	movq	%rax, (%rbx)
	jne	.L50
.L37:
	xorl	%eax, %eax
.L35:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L49:
	.cfi_restore_state
	movl	$1, %eax
	jmp	.L35
.L48:
	leaq	_ZZ13HashTableInitP9HashTableE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC4(%rip), %rdi
	movl	$48, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE91:
	.size	_Z13HashTableInitP9HashTable, .-_Z13HashTableInitP9HashTable
	.p2align 4,,15
	.globl	_Z16HashTableDisinitP9HashTable
	.type	_Z16HashTableDisinitP9HashTable, @function
_Z16HashTableDisinitP9HashTable:
.LFB92:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbp
	xorl	%ebx, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L52:
	movq	0(%rbp), %rax
	movq	(%rax,%rbx), %rdi
	addq	$8, %rbx
	call	free@PLT
	cmpq	$144488, %rbx
	jne	.L52
	movq	0(%rbp), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE92:
	.size	_Z16HashTableDisinitP9HashTable, .-_Z16HashTableDisinitP9HashTable
	.p2align 4,,15
	.globl	_Z13HashTablePushP9HashTablePcPFyPvmE
	.type	_Z13HashTablePushP9HashTablePcPFyPvmE, @function
_Z13HashTablePushP9HashTablePcPFyPvmE:
.LFB93:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA93
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
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
	movq	0(%rbp), %rax
	shrq	$14, %rdx
	imulq	$18061, %rdx, %rdx
	subq	%rdx, %rcx
	movq	(%rax,%rcx,8), %rdi
	call	_Z8ListPushP4ListPc@PLT
	testl	%eax, %eax
	sete	%al
	popq	%rbx
	.cfi_def_cfa_offset 24
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE93:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA93:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE93-.LLSDACSB93
.LLSDACSB93:
.LLSDACSE93:
	.text
	.size	_Z13HashTablePushP9HashTablePcPFyPvmE, .-_Z13HashTablePushP9HashTablePcPFyPvmE
	.section	.rodata.str1.1
.LC5:
	.string	"buffer != NULL"
.LC6:
	.string	"buffer_size > 0"
	.text
	.p2align 4,,15
	.globl	_Z10Hash_FirstPvm
	.type	_Z10Hash_FirstPvm, @function
_Z10Hash_FirstPvm:
.LFB94:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L61
	testq	%rsi, %rsi
	je	.L62
	movzbl	(%rdi), %eax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L61:
	.cfi_restore_state
	leaq	_ZZ10Hash_FirstPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$99, %edx
	call	__assert_fail@PLT
.L62:
	leaq	_ZZ10Hash_FirstPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$100, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE94:
	.size	_Z10Hash_FirstPvm, .-_Z10Hash_FirstPvm
	.p2align 4,,15
	.globl	_Z8Hash_LenPvm
	.type	_Z8Hash_LenPvm, @function
_Z8Hash_LenPvm:
.LFB95:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L67
	testq	%rsi, %rsi
	je	.L68
	movq	%rsi, %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L67:
	.cfi_restore_state
	leaq	_ZZ8Hash_LenPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$108, %edx
	call	__assert_fail@PLT
.L68:
	leaq	_ZZ8Hash_LenPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$109, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE95:
	.size	_Z8Hash_LenPvm, .-_Z8Hash_LenPvm
	.p2align 4,,15
	.globl	_Z8Hash_SumPvm
	.type	_Z8Hash_SumPvm, @function
_Z8Hash_SumPvm:
.LFB96:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L97
	testq	%rsi, %rsi
	je	.L98
	movq	%rdi, %rdx
	leaq	-1(%rsi), %rcx
	movl	$17, %r8d
	negq	%rdx
	andl	$15, %edx
	leaq	15(%rdx), %rax
	cmpq	$17, %rax
	cmovb	%r8, %rax
	cmpq	%rax, %rcx
	jb	.L78
	testq	%rdx, %rdx
	je	.L79
	cmpq	$1, %rdx
	movzbl	(%rdi), %r10d
	je	.L80
	movzbl	1(%rdi), %eax
	addq	%rax, %r10
	cmpq	$2, %rdx
	je	.L81
	movzbl	2(%rdi), %eax
	addq	%rax, %r10
	cmpq	$3, %rdx
	je	.L82
	movzbl	3(%rdi), %eax
	addq	%rax, %r10
	cmpq	$4, %rdx
	je	.L83
	movzbl	4(%rdi), %eax
	addq	%rax, %r10
	cmpq	$5, %rdx
	je	.L84
	movzbl	5(%rdi), %eax
	addq	%rax, %r10
	cmpq	$6, %rdx
	je	.L85
	movzbl	6(%rdi), %eax
	addq	%rax, %r10
	cmpq	$7, %rdx
	je	.L86
	movzbl	7(%rdi), %eax
	addq	%rax, %r10
	cmpq	$8, %rdx
	je	.L87
	movzbl	8(%rdi), %eax
	addq	%rax, %r10
	cmpq	$9, %rdx
	je	.L88
	movzbl	9(%rdi), %eax
	addq	%rax, %r10
	cmpq	$10, %rdx
	je	.L89
	movzbl	10(%rdi), %eax
	addq	%rax, %r10
	cmpq	$11, %rdx
	je	.L90
	movzbl	11(%rdi), %eax
	addq	%rax, %r10
	cmpq	$12, %rdx
	je	.L91
	movzbl	12(%rdi), %eax
	addq	%rax, %r10
	cmpq	$13, %rdx
	je	.L92
	movzbl	13(%rdi), %eax
	addq	%rax, %r10
	cmpq	$14, %rdx
	je	.L93
	movzbl	14(%rdi), %eax
	movl	$15, %ecx
	movl	$15, %r8d
	addq	%rax, %r10
	.p2align 4,,10
	.p2align 3
.L73:
	movq	%rsi, %r11
	pxor	%xmm4, %xmm4
	subq	%rdx, %r11
	pxor	%xmm6, %xmm6
	pxor	%xmm5, %xmm5
	movq	%r11, %rax
	pxor	%xmm2, %xmm2
	shrq	$4, %rax
	addq	%rdi, %rdx
	xorl	%r9d, %r9d
	.p2align 4,,10
	.p2align 3
.L75:
	movdqa	(%rdx), %xmm1
	addq	$1, %r9
	addq	$16, %rdx
	cmpq	%r9, %rax
	movdqa	%xmm1, %xmm3
	punpckhbw	%xmm6, %xmm1
	punpcklbw	%xmm6, %xmm3
	movdqa	%xmm1, %xmm7
	punpckhwd	%xmm5, %xmm1
	movdqa	%xmm3, %xmm0
	punpckhwd	%xmm5, %xmm3
	punpcklwd	%xmm5, %xmm0
	punpcklwd	%xmm5, %xmm7
	movdqa	%xmm0, %xmm8
	punpckhdq	%xmm2, %xmm0
	punpckldq	%xmm2, %xmm8
	paddq	%xmm8, %xmm0
	movdqa	%xmm3, %xmm8
	punpckhdq	%xmm2, %xmm3
	punpckldq	%xmm2, %xmm8
	paddq	%xmm8, %xmm0
	paddq	%xmm3, %xmm0
	movdqa	%xmm7, %xmm3
	punpckldq	%xmm2, %xmm3
	paddq	%xmm3, %xmm0
	movdqa	%xmm7, %xmm3
	punpckhdq	%xmm2, %xmm3
	paddq	%xmm3, %xmm0
	movdqa	%xmm1, %xmm3
	punpckhdq	%xmm2, %xmm1
	punpckldq	%xmm2, %xmm3
	paddq	%xmm3, %xmm0
	paddq	%xmm1, %xmm0
	paddq	%xmm0, %xmm4
	ja	.L75
	movdqa	%xmm4, %xmm0
	movq	%r11, %r9
	andq	$-16, %r9
	psrldq	$8, %xmm0
	paddq	%xmm0, %xmm4
	movq	%xmm4, %rax
	addq	%r9, %r8
	leal	(%rcx,%r9), %edx
	addq	%r10, %rax
	cmpq	%r9, %r11
	je	.L69
.L72:
	movslq	%edx, %rdx
	addq	$1, %rdx
	jmp	.L77
	.p2align 4,,10
	.p2align 3
.L94:
	movq	%rcx, %rdx
.L77:
	movzbl	(%rdi,%r8), %ecx
	movq	%rdx, %r8
	addq	%rcx, %rax
	cmpq	%rdx, %rsi
	leaq	1(%rdx), %rcx
	ja	.L94
.L69:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L83:
	.cfi_restore_state
	movl	$4, %r8d
	movl	$4, %ecx
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L80:
	movl	$1, %r8d
	movl	$1, %ecx
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L81:
	movl	$2, %r8d
	movl	$2, %ecx
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L78:
	xorl	%edx, %edx
	xorl	%eax, %eax
	xorl	%r8d, %r8d
	jmp	.L72
	.p2align 4,,10
	.p2align 3
.L82:
	movl	$3, %r8d
	movl	$3, %ecx
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L84:
	movl	$5, %r8d
	movl	$5, %ecx
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L79:
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	xorl	%r10d, %r10d
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L85:
	movl	$6, %r8d
	movl	$6, %ecx
	jmp	.L73
.L88:
	movl	$9, %r8d
	movl	$9, %ecx
	jmp	.L73
.L86:
	movl	$7, %r8d
	movl	$7, %ecx
	jmp	.L73
.L87:
	movl	$8, %r8d
	movl	$8, %ecx
	jmp	.L73
.L89:
	movl	$10, %r8d
	movl	$10, %ecx
	jmp	.L73
.L90:
	movl	$11, %r8d
	movl	$11, %ecx
	jmp	.L73
.L91:
	movl	$12, %r8d
	movl	$12, %ecx
	jmp	.L73
.L92:
	movl	$13, %r8d
	movl	$13, %ecx
	jmp	.L73
.L93:
	movl	$14, %r8d
	movl	$14, %ecx
	jmp	.L73
.L98:
	leaq	_ZZ8Hash_SumPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$116, %edx
	call	__assert_fail@PLT
.L97:
	leaq	_ZZ8Hash_SumPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$115, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE96:
	.size	_Z8Hash_SumPvm, .-_Z8Hash_SumPvm
	.p2align 4,,15
	.globl	_Z11Hash_RotatePvm
	.type	_Z11Hash_RotatePvm, @function
_Z11Hash_RotatePvm:
.LFB97:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L105
	testq	%rsi, %rsi
	je	.L106
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L102:
	movzbl	(%rdi), %edx
	salq	$4, %rax
	addq	$1, %rdi
	xorq	%rdx, %rax
	cmpq	%rdi, %rsi
	jne	.L102
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L105:
	.cfi_restore_state
	leaq	_ZZ11Hash_RotatePvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$128, %edx
	call	__assert_fail@PLT
.L106:
	leaq	_ZZ11Hash_RotatePvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$129, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE97:
	.size	_Z11Hash_RotatePvm, .-_Z11Hash_RotatePvm
	.p2align 4,,15
	.globl	_Z8Hash_GNUPvm
	.type	_Z8Hash_GNUPvm, @function
_Z8Hash_GNUPvm:
.LFB98:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L113
	testq	%rsi, %rsi
	je	.L114
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L110:
	movzbl	(%rdi), %edx
	leal	5(%rax), %ecx
	addq	$1, %rdi
	salq	%cl, %rax
	addq	%rdx, %rax
	cmpq	%rdi, %rsi
	jne	.L110
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L113:
	.cfi_restore_state
	leaq	_ZZ8Hash_GNUPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$142, %edx
	call	__assert_fail@PLT
.L114:
	leaq	_ZZ8Hash_GNUPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$143, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE98:
	.size	_Z8Hash_GNUPvm, .-_Z8Hash_GNUPvm
	.p2align 4,,15
	.globl	_Z9Hash_FAQ6Pvm
	.type	_Z9Hash_FAQ6Pvm, @function
_Z9Hash_FAQ6Pvm:
.LFB99:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L121
	testq	%rsi, %rsi
	je	.L122
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L118:
	movzbl	(%rdi), %edx
	addq	$1, %rdi
	addq	%rax, %rdx
	movq	%rdx, %rax
	salq	$10, %rax
	addq	%rdx, %rax
	movq	%rax, %rdx
	shrq	$6, %rdx
	xorq	%rdx, %rax
	cmpq	%rdi, %rsi
	jne	.L118
	leaq	(%rax,%rax,8), %rdx
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rdx, %rax
	shrq	$11, %rax
	xorq	%rdx, %rax
	movq	%rax, %rdx
	salq	$15, %rdx
	addq	%rdx, %rax
	ret
.L121:
	.cfi_restore_state
	leaq	_ZZ9Hash_FAQ6PvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$155, %edx
	call	__assert_fail@PLT
.L122:
	leaq	_ZZ9Hash_FAQ6PvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$156, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE99:
	.size	_Z9Hash_FAQ6Pvm, .-_Z9Hash_FAQ6Pvm
	.p2align 4,,15
	.globl	_Z10Hash_Rot13Pvm
	.type	_Z10Hash_Rot13Pvm, @function
_Z10Hash_Rot13Pvm:
.LFB100:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L129
	testq	%rsi, %rsi
	je	.L130
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L126:
	movzbl	(%rdi), %edx
	addq	$1, %rdi
	addq	%rdx, %rax
	movq	%rax, %rdx
	movq	%rax, %rcx
	salq	$13, %rdx
	shrq	$19, %rcx
	orq	%rcx, %rdx
	subq	%rdx, %rax
	cmpq	%rdi, %rsi
	jne	.L126
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L129:
	.cfi_restore_state
	leaq	_ZZ10Hash_Rot13PvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$174, %edx
	call	__assert_fail@PLT
.L130:
	leaq	_ZZ10Hash_Rot13PvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$175, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE100:
	.size	_Z10Hash_Rot13Pvm, .-_Z10Hash_Rot13Pvm
	.p2align 4,,15
	.globl	_Z7Hash_LyPvm
	.type	_Z7Hash_LyPvm, @function
_Z7Hash_LyPvm:
.LFB101:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L137
	testq	%rsi, %rsi
	je	.L138
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L134:
	movzbl	(%rdi), %edx
	addq	$1, %rdi
	imulq	$1664525, %rax, %rax
	cmpq	%rdi, %rsi
	leaq	1013904223(%rax,%rdx), %rax
	jne	.L134
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L137:
	.cfi_restore_state
	leaq	_ZZ7Hash_LyPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$189, %edx
	call	__assert_fail@PLT
.L138:
	leaq	_ZZ7Hash_LyPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$190, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE101:
	.size	_Z7Hash_LyPvm, .-_Z7Hash_LyPvm
	.p2align 4,,15
	.globl	_Z7Hash_RsPvm
	.type	_Z7Hash_RsPvm, @function
_Z7Hash_RsPvm:
.LFB102:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L145
	testq	%rsi, %rsi
	je	.L146
	addq	%rdi, %rsi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L142:
	movzbl	(%rdi), %edx
	addq	$1, %rdi
	imulq	$63689, %rax, %rax
	addq	%rdx, %rax
	cmpq	%rdi, %rsi
	jne	.L142
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L145:
	.cfi_restore_state
	leaq	_ZZ7Hash_RsPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$202, %edx
	call	__assert_fail@PLT
.L146:
	leaq	_ZZ7Hash_RsPvmE19__PRETTY_FUNCTION__(%rip), %rcx
	leaq	.LC3(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$203, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE102:
	.size	_Z7Hash_RsPvm, .-_Z7Hash_RsPvm
	.section	.rodata
	.align 16
	.type	_ZZ7Hash_RsPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ7Hash_RsPvmE19__PRETTY_FUNCTION__, 30
_ZZ7Hash_RsPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Rs(void*, size_t)"
	.align 16
	.type	_ZZ7Hash_LyPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ7Hash_LyPvmE19__PRETTY_FUNCTION__, 30
_ZZ7Hash_LyPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Ly(void*, size_t)"
	.align 32
	.type	_ZZ10Hash_Rot13PvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ10Hash_Rot13PvmE19__PRETTY_FUNCTION__, 33
_ZZ10Hash_Rot13PvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Rot13(void*, size_t)"
	.align 32
	.type	_ZZ9Hash_FAQ6PvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ9Hash_FAQ6PvmE19__PRETTY_FUNCTION__, 32
_ZZ9Hash_FAQ6PvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_FAQ6(void*, size_t)"
	.align 16
	.type	_ZZ8Hash_GNUPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ8Hash_GNUPvmE19__PRETTY_FUNCTION__, 31
_ZZ8Hash_GNUPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_GNU(void*, size_t)"
	.align 32
	.type	_ZZ11Hash_RotatePvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ11Hash_RotatePvmE19__PRETTY_FUNCTION__, 34
_ZZ11Hash_RotatePvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Rotate(void*, size_t)"
	.align 16
	.type	_ZZ8Hash_SumPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ8Hash_SumPvmE19__PRETTY_FUNCTION__, 31
_ZZ8Hash_SumPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Sum(void*, size_t)"
	.align 16
	.type	_ZZ8Hash_LenPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ8Hash_LenPvmE19__PRETTY_FUNCTION__, 31
_ZZ8Hash_LenPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_Len(void*, size_t)"
	.align 32
	.type	_ZZ10Hash_FirstPvmE19__PRETTY_FUNCTION__, @object
	.size	_ZZ10Hash_FirstPvmE19__PRETTY_FUNCTION__, 33
_ZZ10Hash_FirstPvmE19__PRETTY_FUNCTION__:
	.string	"Hash_t Hash_First(void*, size_t)"
	.align 16
	.type	_ZZ13HashTableInitP9HashTableE19__PRETTY_FUNCTION__, @object
	.size	_ZZ13HashTableInitP9HashTableE19__PRETTY_FUNCTION__, 30
_ZZ13HashTableInitP9HashTableE19__PRETTY_FUNCTION__:
	.string	"int HashTableInit(HashTable*)"
	.align 16
	.type	_ZZ9PrintListP4ListE19__PRETTY_FUNCTION__, @object
	.size	_ZZ9PrintListP4ListE19__PRETTY_FUNCTION__, 22
_ZZ9PrintListP4ListE19__PRETTY_FUNCTION__:
	.string	"void PrintList(List*)"
	.align 16
	.type	_ZZ8ListInitP4ListE19__PRETTY_FUNCTION__, @object
	.size	_ZZ8ListInitP4ListE19__PRETTY_FUNCTION__, 21
_ZZ8ListInitP4ListE19__PRETTY_FUNCTION__:
	.string	"void ListInit(List*)"
	.local	_ZZ9LListPushP4ListPcE12number_calls
	.comm	_ZZ9LListPushP4ListPcE12number_calls,4,4
	.globl	memory
	.bss
	.align 32
	.type	memory, @object
	.size	memory, 53294832
memory:
	.zero	53294832
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
