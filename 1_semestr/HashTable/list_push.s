	.local	_ZZ8ListPushP4ListPcE12number_calls
	.comm	_ZZ8ListPushP4ListPcE12number_calls,4,4
	.globl	_Z8ListPushP4ListPc
	.type	_Z8ListPushP4ListPc, @function
_Z8ListPushP4ListPc:
.LFB17:
	# new_node = memory + number_calls++;	
   	movq	_ZZ8ListPushP4ListPcE12number_calls(%rip), %rax
	leal	1(%rax), %edx
	movl	%edx, _ZZ8ListPushP4ListPcE12number_calls(%rip)
	salq	$4, %rax
	leaq	memory(%rip), %rdx
	addq	%rax, %rdx

    # new_node->data = value;
    # new_node->next = NULL;
	movq	%rsi, (%rdx)
	movq	$0, 8(%rdx)    
        
    # if (list->number_nodes == 0) {
	movq	8(%rdi), %rax
	testq	%rax, %rax	
    	jne	.L11
    
    # list->number_nodes += 1;	
	addq    $1, %rax
	movq	%rax, 8(%rdi)

    # list->first_elem = list->last_elem = new_node;
	movq	%rdx, 16(%rdi)
	movq	%rdx, (%rdi)
	
    	xorl	%eax, %eax
	ret
.L11:
    # list->number_nodes += 1;	
	addq    $1, %rax
	movq	%rax, 8(%rdi)

    # list->last_elem->next = new_node;
    # list->last_elem = new_node;    
    	movq	16(%rdi), %rax
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rdi)

    	xorl	%eax, %eax
	ret
.LFE17:
	.size	_Z8ListPushP4ListPc, .-_Z8ListPushP4ListPc
