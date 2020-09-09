DEFAULT REL

section .text

;====================================================
; Do printf function
;
; Expect: format string on th   e top of the stack and
;         arguments for format string
; Destroy: rax, rbx, rdx, rcx, rdi, rsi, r15, r14, r12, r9
;==================================================== 
global printf

printf:             
            pop r9                   ; ret parameter
            pop rsi
            
printf_loop:
            lodsb               ; get new char
            or rax, rax         ; if end of string
            je end_printf       ;
            
            cmp rax, '%'        ; if special symbol
            je analyse_special
            cmp rax, '\'        ; if escape sequance
            je escape_symbol
            dec rsi             ; if symbol
            call putc           ; just prnit it
            inc rsi             ;
            jmp printf_loop
            
analyse_special:
            lodsb               ; read special symbol
            pop rbx             ; get argument
            
            cmp rax, 'd'        ; decimal number
            jne continue0       ;
            mov r12, rsi        ; 
            call dtos           ;
            mov rsi, r12        ; 
            jmp printf_loop     ;
            
continue0:  cmp rax, 'o'        ; octal number
            jne continue1       ;
            mov r12, rsi        ;
            mov r13, 8          ;
            call ntos           ;      
            mov rsi, r12        ;        
            jmp printf_loop     ;
            
continue1:  cmp rax, 'x'        ; hexadecimal number
            jne continue2       ;
            mov r12, rsi        ; 
            mov r13, 16         ;
            call ntos           ;
            mov rsi, r12        ; 
            jmp printf_loop     ;
            
continue2:  cmp rax, 'b'        ; binary number
            jne continue3       ;
            mov r12, rsi        ;
            mov r13, 2          ;
            call ntos           ;
            mov rsi, r12        ; 
            jmp printf_loop     ;
            
continue3:  cmp rax, 's'        ; string
            jne continue4       ;
            mov r12, rsi        ; 
            mov rsi, rbx        ; for puts
            call puts           ;
            mov rsi, r12        ; 
            jmp printf_loop     ;
            
continue4:  cmp rax, 'c'        ; char
            jne continuee1      ;
            mov r12, rsi        ; 
            mov rsi, rbx        ; for putc
            call putc           ; 
            mov rsi, r12        ;
            jmp printf_loop     ;

continuee1: cmp rax, 'C'        ; Char
            jne continue5       ;
            mov r12, rsi        ; 
            mov cl, bl          ; for putC
            call putC           ; 
            mov rsi, r12        ;
            jmp printf_loop     ;
                                  
continue5:  cmp rax, '%'        ; % symbol
            jne error_loop      ;
            push rbx            ;
            dec rsi             ; for putc
            call putc           ;
            inc rsi             ;
            jmp printf_loop     ;

error_loop:
            mov rsi, error_msg  ; error message
            jmp end_printf      ;


%macro  PRINT_ESCAPE_AND_CONTINUE 1 

            mov rsi, escape + %1;
            call putc           ; print escape
            mov rsi, r15        ; restore rsi
            jmp printf_loop     ;

%endmacro

escape_symbol:
            lodsb               ; read special symbol
            mov r15, rsi        ; save rsi
                
            cmp rax, 'n'        ; new_line
            jne continue6       ;
            PRINT_ESCAPE_AND_CONTINUE(0)
            
continue6:  cmp rax, 't'        ; tab
            jne continue7       ;
            PRINT_ESCAPE_AND_CONTINUE(1)

continue7:  cmp rax, 'b'        ; backspace
            jne continue8       ;
            PRINT_ESCAPE_AND_CONTINUE(2)
            
continue8:  cmp rax, 'v'        ; vertical tab
            jne continue9       ;
            PRINT_ESCAPE_AND_CONTINUE(3)
            
continue9:  cmp rax, '\'        ; \ symbol
            jne error_loop      ;
            PRINT_ESCAPE_AND_CONTINUE(4)          

end_printf:
            push r9
            ret
