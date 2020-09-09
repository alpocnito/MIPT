section .text


;====================================================
; Convert decimal number to 2^N base representation
;  and print it in stdout
;
; Expect: rbx - number
;         r13 - base. Only 2, 4, 8, 16!
; Destroy: r15, r14, r13, rdx, rax, rcx, rbx, rdi
;====================================================
ntos:       
            dec r13
            call count_ones     ; now r15 = N
            mov r14, 64         ; r14 = 64 - N
            sub r14, r15
            
            mov rcx, 0          ; for shl
            mov rdi, data       ; for stosb
            mov rdx, rbx        ; save rbx
             
            cmp r15, 3          ; if (base == 8)
            jne loop_ntos       ;
            shr rbx, 63         ; analyse first bit of
                                ; 8 base word
            mov al, bl          ;
            mov rbx, DIG16      ;
            xlat                ; 
            stosb               ;
            inc rcx             ;
            
loop_ntos:
            mov rbx, rdx        ; restore rbx
            shl rbx, cl         ; take the 4 least bits
            mov rax, rcx        ; sace rcx
            mov rcx, r14
            shr rbx, cl         ; move them to right of word
            mov rcx, rax        ; restore rcx
            
            mov al, bl          ;
            mov rbx, DIG16      ; al = DIG16[AL]
            xlat                ; 
            
            stosb               ; store al
            
            add rcx, r15        ; rcx += N
            cmp rcx, 64         ;
            je .end_loop
            jmp loop_ntos
.end_loop:             
            
            mov rdi, data       ; for scasb 
            xor rdx, rdx
            mov rax, 64         ; rax = 64 / N
            div r15
            mov rcx, rax        ; len data
            add rcx, rdx
          
            mov al, 0x30        ; '0'
            repe scasb          ; skip '0'
            
            inc cx              ; len of buffer
            dec rdi
            mov rsi, rdi        ; buffer
            call putbuf  


            ret

;====================================================
; Count number of one's in rdx
;
; Expect: r13 - given number
; Return: r15 - number of one's
; Destroy:  
;==================================================== 
count_ones:
            mov rcx, 64     ; number of bits in rbx
            xor r15, r15    ; for answer
count_ones_l:
            test r13, 1     ; test Last bit
            jz .continue
            inc r15
.continue:  
            shr r13, 1      ; shifr rbx
            loop count_ones_l
            ret



;====================================================
; Print decimal number in stdout
;
; Expect: rbx - number
; Destroy: rax, rbx, rdx, rcx, rdi, rsi 
;====================================================  
dtos:    
            mov rdi, data       ; for stosb
            mov rsi, 10         ; for div
            mov rax, rbx
            
next_dnum:
            xor rdx, rdx         
            div rsi             ; rax /= 10
            mov rcx, rax        ; save rax
                
            mov al, dl
            add al, 0x30        ; 0x30
            stosb
            mov rax, rcx        ; restore rax
            
            or rax, rax
            je .end_loop
            jmp next_dnum
.end_loop:            

            dec rdi
            mov rsi, rdi
.print_loop:
            call putc
            dec rsi       
            cmp rsi, data
            jge .print_loop 
            
            ret
;====================================================



;====================================================
; Print one symbol in stdout
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
; Print one symbol in stdout
;
; Expect: cl - symbol
; Destroy: rax, rdi, rdx, rsi
;====================================================
putC:       
            sub rsp, 100
            mov BYTE [rsp], cl
            mov rsi, rsp

            mov rax, 0x01      ; write
            mov rdi, 1         ; stdout
            mov rdx, 1         ; length
            syscall
                   
            add rsp, 100
            
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
; Print symbols from address rsi, while not '\0' 
;
; Expect: rsi - address of the string
; Destroy: rax, rdi, rdx 
;====================================================
puts:       
.next_char:
            cmp byte [rsi], byte 0
            je .end_loop
          
            call putc
            inc rsi
            jmp .next_char         
.end_loop:  
                  
            ret
;====================================================
DEFAULT REL
section .data
test2:      db "Sampl\\e string\nTab:\t.\nVertical tab:\v.\nBackspace:\b.\nDecimal: %d\nOctal: %o\nBinary: %b\nHexadecimal: %x\nString: %s\nSymbol: %c", 0
test1:      db 0
new_test    db "------------------------------------------------------------------------------------------------------------------------------------------", 0
DEDstr1:    db "Love", 0
DEDstr2:    db "%C %s %x %d %% %b%b", 0
str2:       db "LongLongString", 0
str1:       db 0

space:      db " " 
DIG16:      db '0123456789ABCDEF'
error_msg:  db 0xA, "Undefined symbol", 0xA
escape:     db 0xA, 9, 9, 0xB, 0x5C, '\'


data:       dq 0
           
           
