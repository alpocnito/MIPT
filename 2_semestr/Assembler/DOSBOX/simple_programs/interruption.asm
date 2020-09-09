;-------------------------------------------------------
;
; Program catch keyboard interruption, and print all 
; 	pressed keys in stdout
;
;-------------------------------------------------------

.model tiny
.code
.186
locals @@
org 100h
Start:		jmp Program



;--------------------------------------------------------------
;
; 9 interruption catcher
;
;--------------------------------------------------------------
New09		proc
		push ax di es dx

		mov di, 0B800h		; di = B800
		mov es, di		; es = di
		mov di, (5*80+40)*2	; di = screen_center
		
		in al, 60h		; al = key_number
		mov ah, 04Eh		; ah = color


;
;		Converting Hex number to ASCII				
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		mov dl, al             	; dl = al                
		shr al, 4		; al >> 4                
		                                                 
		mov dh, al                                       
		cmp al, 9h		; if (al > 9)
		jg Letter1              ;   dh = al + 'A'
		add dh, 30h             ; else 
		jmp Next1               ;   dh = al + '0'
Letter1:	add dh, 41h - 0Ah		
Next1:      
		and dl, 0Fh        
		cmp dl, 9h		; if (dl > 9)
		jg Letter2              ;   dl = dl + 'A'
		add dl, 30h             ; else 
		jmp Next2
Letter2:	add dl, 41h - 0Ah	;   dl = dl + '0'
Next2:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


		mov al, dh              ; show key
		stosw			; in hex
		mov al, dl              ; format
		stosw

		mov al, 68h		; 'h'
		stosw	
             
		pop dx es di ax            

		db 0eah                 ; chain to the hardware interrupt
old09		dw 0                    ;
		dw 0                    ;
		
		endp
;--------------------------------------------------------------



				
Program:	xor ax, ax				; ax = 0
		mov es, ax				; es = ax
		mov bx, 9 * 4				; bx = 9 interrupt
		
		mov ax, es:[bx]				;  
		mov old09, ax                           ; old09 = es[bx]
		mov ax, es:[bx + 2]                     ;
		mov old09 + 2, ax                       ; old09 + 2 = es[bx + 2]2222

												
		cli					; stop_int
		mov word ptr es:[bx], offset New09	; es[bx]   = int_function
		mov ax, cs                            
		mov word ptr es:[bx+2], ax		; es[bx+2] = int_function
		sti					; start_int

		mov ah, 31h		; ah = 31h
		mov dx, offset TheEND	; dx = &TheEND
		shr dx, 4
		inc dx
		int 21h			; end program

TheEND:		end Start	R
