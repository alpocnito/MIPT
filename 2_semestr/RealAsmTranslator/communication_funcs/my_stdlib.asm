section .text
global _start

jmp _start

;================== JUMP TABLE ======================
get_shell: jmp get
put_shell: jmp put
root_shell:jmp root
;====================================================


;====================================================
; Read int, convert to float
; 
; Destroy: ALL
; Return: xmm0 - answer
;====================================================
get:
    sub rsp, 64
    call read
    
    mov rdi, rsp       ; save
    
    mov r12, 0         ; for user dot
    call stod
    add rsp, 64
    
    call pwr10         ; 10^(r12)
    pxor xmm7, xmm7    ; 
    cvtsi2sd xmm7, rax ; xmm7 = rax
    cvtsi2sd xmm6, r12 ; 
    divsd xmm7, xmm6   ; xmm7 / r12
    
    pop r12

    sub rsp, 8         ; push ans
    movsd qword [rsp], xmm7
    
    push r12
    ret
;====================================================

;====================================================
; Print decimal number in stdout
;
; Expect: number from stack
; Destroy: rax, rbx, rdx, rcx, rdi, rsi, r12, r11, r10
;====================================================  
put:        
    pop r11
            
    movsd qword xmm7, [rsp]
    mov r10, 100000
    cvtsi2sd xmm6, r10
    mulsd xmm7, xmm6
    cvttsd2si rbx, xmm7

    push r11
    
    cmp rbx, 0xFFFFFFFF ; if 2's compliment
    jg .norm_nuber
    xor rbx, 0xFFFFFFFFFFFFFFFF
    inc rbx
    
    mov rsi, rsp
    sub rsi, 60         ;
    mov BYTE [rsi], "-" ; print(-)
    call putc           ;




.norm_nuber:
    sub rsp, 64

    mov rdi, rsp        ; for stosb
    mov rsi, 10         ; for div
    mov rax, rbx        ; main number
    mov r10, 0          ; for dot

.next_dnum:
    
    xor rdx, rdx         
    div rsi             ; rax /= 10
    mov rcx, rax        ; save rax
                
    mov al, dl
    add al, 0x30        ; 0x30
    stosb
    mov rax, rcx        ; restore rax
            
    or rax, rax
    je .end_loop
    jmp .next_dnum
.end_loop:            

    dec rdi             ; print
    mov rsi, rdi        ; digits
    
    call put_number
    
    add rsp, 64

    jmp rett
    new_line: db 0xA
rett:

    mov rsi, new_line
    call putc
    ret
;====================================================


;====================================================
; 10 ^ (r12)
; If r12 is greater than 100, return 1
;
; Expect: r12
; Destroy: rbx, r12
; Return: r12 - answer
;====================================================
pwr10:
    cmp r12, 100
    jg .zero

    mov rbx, 1
    cmp r12, 0
.loop:
    je .ret
    imul rbx, rbx, 10
    dec r12
    jmp .loop

.ret:
    cmp rbx, 1
    je .zero
    mov r12, rbx
    jmp .not_zero
.zero:
    mov r12, 1
.not_zero:
    ret
;====================================================
 


;====================================================
; Read from stdin and put in rsp
;
; Expect:
; Destroy: rax, rdi, rsi, rdx, r9, r8
; Return: rsi - number read symbols
;====================================================
read:
    pop r8

    mov rax, 0   
    mov rdi, 0
    mov rdx, 64
    mov rsi, rsp
    syscall
    
    ; Find last symbol
    xor rsi, rsi
    mov r9, rsp
.loop:
    cmp BYTE [r9], "."
    je .dot
    cmp BYTE [r9], "-"
    je .dot
    cmp BYTE [r9], "0"
    jl .end
    cmp BYTE [r9], "9"
    jg .end
 
 .dot:
    inc rsi
    inc r9
    jmp .loop
.end:
    push r8
    ret
;====================================================
 

;====================================================
; Expect:
;   rsi = size of input
;   rdi = pointer to input
; Return:
;   r12  = digits after dot
; Destroy: rax, rcx, rsi, r9, r8, r10
;====================================================
stod:
    mov r10, 0    ; for minus
    xor r9, r9    ; ans
    mov rax, 1    ; exponent
    mov r8, rdi   ; main pointer
    add r8, rsi   
    dec r8
    mov r12, r8   ; digits after dot
    
.loop:  
    cmp rdi, r8
    jg  .end
        
    xor rcx, rcx
    mov cl, BYTE [r8]
        
    cmp cl, "."   ;
    jne .cont     ;
    sub r12, r8   ; if dot is found
    dec r8        ;
    jmp .loop     ;
.cont:
    cmp cl, "-"   ;
    jne .number   ;
    mov r10, 1    ; if minus is found
    dec r8        ;
    jmp .loop     ;


.number:
    sub cl, "0"
    imul rcx, rax
    add r9, rcx
    imul rax, rax, 10
        
    dec r8
    jmp .loop
.end: 
    
    cmp r10, 0
    je .pos
    xor r9, 0xFFFFFFFFFFFFFFFF
    mov rax, r9
    inc rax
    ret
.pos:
    mov rax, r9 
    
    ret
;====================================================


;====================================================
; Print number from stack with dot
;
; Expect: 
;     rsi - address of the first symbol
;     rsp - address ot the last symbol
; Destroy: rsi, r10, 
;====================================================
put_number:
    pop r12

    sub rsi, 5          ;
    cmp rsi, rsp        ;
    jge .print_loop_start;
    add rsi, 6          ;
    mov BYTE [rsi], "0" ; print(0.)
    call putc           ;
    mov BYTE [rsi], "." ;
    call putc           ;
    dec rsi             ;
    
    sub rsi, 5          ;
    mov r10, rsp        ;
    sub r10, rsi        ;
    add rsi, 6          ;
    mov BYTE [rsi], "0" ; Leading zeros:
.new_zero:              ; print(000...) 
    cmp r10, 1          ;
    jle .end_zero       ;
    call putc           ;
    dec r10             ;
    jmp .new_zero       ;
.end_zero:
    sub rsi, 6
.print_loop_start:
    add rsi, 5
.print_loop:
    call putc
    
    
    sub rsi, 5          ; 
    cmp rsi, rsp        ;
    jne .no_dot         ; If dot:
    add rsi, 5          ; print(.)
    mov BYTE [rsi], "." ;
    call putc           ;
    sub rsi, 5          ;

.no_dot:
    add rsi, 4

    cmp rsi, rsp
    jge .print_loop 
    
    push r12
    ret
;====================================================

;====================================================
; Print number in stdout from stack
;
; Expect: rsi - address of the symbol
; Destroy: rax, rdi, rdx 
;====================================================
putc:
    mov rax, 0x01      ; write
    mov rdi, 1         ; stdout
    mov rdx, 1         ; length
    syscall
                   
            
    ret
;====================================================


;====================================================
; Print n symbols in stdout
;
; Expect: rsi - address of the symbol
;         rcx - number of symbols
; Destroy: rax, rdi, rdx 
;====================================================
putbuf:
    mov rax, 0x01      ; write
    mov rdi, 1         ; stdout
    mov rdx, rcx       ; length
    syscall
               
    ret
;====================================================


;====================================================
; Take root from number
;
; Expect: xmm7 - number
; Return: xmm6 - answer
; Destroy: xmm7, xmm6, xmm5, xmm4, rsi 
;====================================================
root: 
    mov rsi, 1          ;
    cvtsi2sd xmm6, rsi  ;
    mov rsi, 2          ;
    cvtsi2sd xmm5, rsi  ; xmm4 = 1/2
    divsd xmm6, xmm5    ;
    movsd xmm4, xmm6    ;

    movsd xmm5, xmm7    ; X0
    mov rsi, 50          ; counter

.loop:
    movsd xmm6, xmm7    ; xmm6 = a
    divsd xmm6, xmm5    ; a / Xn
    addsd xmm6, xmm5    ; Xn + a/Xn
    mulsd xmm6, xmm4    ; 1/2 * (Xn + a/Xn)

    movsd xmm5, xmm6
    
    cmp rsi, 0
    je .ret
    sub rsi, 1
    jmp .loop

.ret:
    movsd xmm7, xmm5
    ret
;====================================================


_start:

    sub rsp, 800
    mov rbp, rsp
    
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    
    call get_shell
    call root_shell
    call put_shell

    mov rax, 60  ; use the _exit syscall
    mov rdi, 0   ; error code 0
    syscall      ; make syscall

