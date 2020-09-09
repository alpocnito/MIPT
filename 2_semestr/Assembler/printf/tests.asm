%include "helpful_functions.asm"
%include "main_loop.asm"

section .text

global _start
_start:
            
            ; btos tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rbx, 0
            mov r13, 2
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 1
            mov r13, 2
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 0b0100_1111_0100_1111
            mov r13, 2
            call ntos
            mov rsi, space
            call putc
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            
            ; htos tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rbx, 0
            mov r13, 16
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 1
            mov r13, 16
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 0x0123456789ACDEF
            mov r13, 16
            call ntos
            mov rsi, space
            call putc
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            
            
            ; otos tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rbx, 0
            mov r13, 8
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 1
            mov r13, 8
            call ntos
            mov rsi, space
            call putc
            
            mov rbx, 0o00123456776
            mov r13, 8
            call ntos
            mov rsi, space
            call putc
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            
            
            ; dtos tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rbx, 0
            call dtos
            mov rsi, space
            call putc
            
            mov rbx, 1
            call dtos
            mov rsi, space
            call putc
            
            mov rbx, 000123456789009876 
            call dtos
            mov rsi, space
            call putc
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
                      
            
            ; puts tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, str1 
            call puts
            mov rsi, space
            call putc
            
            mov rsi, str2 
            call puts
            mov rsi, space
            call putc
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            
            ; printf tests
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            push str2
            push str2
            push 228
            push 228
            push 228
            push 228  
            push test2
            call printf
             
            ;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            push test1
            call printf
            ;;;;;;;;;;;;;;;;;;;;;;;
            
            push str2
            push str2
            push 0xFFFFFFFFFFFFFFFF
            push 0xFFFFFFFFFFFFFFFF
            push 0xFFFFFFFFFFFFFFFF
            push 0xFFFFFFFFFFFFFFFF  
            push test2
            call printf
            
            ;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            push test1
            call printf
            ;;;;;;;;;;;;;;;;;;;;;;;
                        
            push str2
            push str2
            push 0
            push 0
            push 0
            push 0  
            push test2
            call printf
            
            ;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            push test1
            call printf
            ;;;;;;;;;;;;;;;;;;;;;;;
            
            push str2
            push str2
            push 1
            push 1
            push 1
            push 1  
            push test2
            call printf
            
            ;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
            
            push test1
            call printf
            ;;;;;;;;;;;;;;;;;;;;;;;
            
            push 127
            push '!'
            push 100d
            push 3802d
            push DEDstr1
            push 'I'
            push DEDstr2
            call printf 
            ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            mov rsi, escape
            call putc
            
            push new_test
            call printf

            mov rsi, escape
            call putc
          
            
            
            mov rax, 0x3C      ; exit
            xor rdi, rdi
            syscall
    
