.model tiny
.186
.code
org 100h                  1	

Start:		jmp Program
		include lib.asm


NL EQU 0Ah 	; new line

;-----------------------------------------------------
; Prints data, while NOT '$'
; 
; Destroy: bx, ah, dl
;-----------------------------------------------------
Print		proc
 		mov bx, offset data	; bx = &data
                mov ah, 02h             ; ah = 2
Loop_s:         mov dl, [b x]           ; dl = *bx
                cmp dl, '$'             ;
                je Loop_e               ; print, while
                int 21h                 ; NOT '$'
                inc bx                  ;
                jmp Loop_s              ;
Loop_e:
		mov dl,	NL		; print \n 
		int 21h

		ret
                endp
;-----------------------------------------------------
                                   	

 
Program:	mov al, 4Ch		; Checking memset
		mov cx, 0               ;
		mov dx, cs              ;
		mov es, dx              ;        1
		mov di, offset data     ;
                call memset             ;
                call print

                mov al, 4Ch		; Checking memset
		mov cx, 10              ;        
		mov di, offset data     ;        
                call memset             ;        1
                call print


                mov al, 4Ch		; Checking memchr
		mov cx, 10              ;        
		mov di, offset data     ;        
                call memchr             ;        1
                mov byte ptr es:[di], '+'
                call print              ;

		mov al, 'A'		; Checking memchr
		mov cx, 15            	;        
		mov di, offset data     ;
                call memchr             ;        2
                mov byte ptr es:[di], '+'
                call print              ;

                mov al, ';'		; Checking memchr
		mov cx, 15              ;        
		mov di, offset data     ;
                call memchr             ;        3
                mov word ptr data, di   ;
                call print              ;


		mov cx, 3               ; Checking memcpy       
		mov di, offset data     ;
		mov si, offset data	;        
                add si, 14              ;        1
                call memcpy             ;        
                call print              ;

                mov cx, 3               ; Checking memcpy       
		mov di, offset data     ;
		mov si, offset data	;
		add si, 4               ;        2
                call memcpy             ;        
                call print              ;


                mov cx, 3               ; Checking memcmp       
		mov di, offset data     ;
		mov si, offset data	;        1
                call memcmp             ;
                mov word ptr data, cx	;        
                call print              ;

                mov cx, 3               ; Checking memcmp       
		mov di, offset data     ;
		mov si, offset data	;
		add si, 4               ;        2
                call memcmp             ;
                mov word ptr data, cx	;        
                call print              ;

                mov cx, 3               ; Checking memcmp       
		mov di, offset data     ;
		mov si, offset data	;
		add di, 5               ;        3
                call memcmp             ;
                mov word ptr data, cx	;        
                call print              ;


                mov di, offset data 		 ; insert
                mov byte ptr es:[di + 8], 0      ;   null char

                mov di, offset data     ; Checking strlen       
                call strlen             ;
                add si, '0'		;        1
                mov word ptr data, si	;       
                call print              ;
                
                mov di, offset data 		; insert
                mov byte ptr es:[di], 0      	;   null char

                mov di, offset data     ; Checking strlen             
                call strlen             ;
                add si, '0'		;        2
                mov word ptr data, si	;        
                call print
                

                mov al, 4Ch		; al = 'L'
		mov cx, 4               ; filling data
		mov di, offset data     ; with 'L'
                call memset             ;
           	mov di, offset data     ; data[4] = 'A'
                mov byte ptr es:[di+4], 'A'  

                mov di, offset data     ; Checking strchr
                mov al, 'A'		;             
                call strchr             ;        
                mov byte ptr es:[di],'+';       1
                call print              ;

               	mov di, offset data     ; Checking strchr
                mov al, 'F'		;             
                call strchr             ;        
                mov word ptr data, di	;        2
                call print              ;


                mov al, 4Ch		; al = 'L'
		mov cx, 4               ; filling data
		mov di, offset data     ; with 'L'
                call memset             ;
           	mov di, offset data     ; data[4] = 'A'
                mov byte ptr es:[di+4], 'A'
                
                mov di, offset data     ; Checking strrchr
                mov al, 'A'		;             
                call strrchr            ;        
                mov byte ptr es:[si],'+';       1
                call print              ;

               	mov di, offset data     ; Checking strrchr
                mov al, 'F'		;             
                call strrchr            ;        
                mov word ptr data, si	;       2
                call print              ;
                

                mov di, offset data 		; insert
                mov byte ptr es:[di + 20], 0    ;   null char
       
		mov di, offset data     ; Checking strcpy
		mov si, offset data	;
                add si, 10              ;        
                call strcpy             ;        1
                call print              ;
                
                jmp strstr_test     			; creating
                new_data db 'abbbaba', 0, 'ba', 0       ;  new string
     
strstr_test:	mov di, offset data
		mov si, offset new_data
		mov cx, 12
		call memcpy 
		call print
		mov di, offset data     ; Checking strstr
		mov si, offset data	;
                add di, 8               ;        
                call strstr             ;        1
                mov si, ax
                mov byte ptr es:[si], '+'
                call print
                


               	mov ax, 4C00h           ; Exit
		int 21h                 ;
data:		db  "SOME STUPID DATA LALALA", 10 dup("$")	
end 		Start
