
;-----------------------------------------------------
;
; Program print pretty table on the screen, which can
;	change shadow color by pressing random key 
;
;-----------------------------------------------------

.model tiny			
.code
locals @@
.186
org 100h
Start:	jmp	 Program

VIDEO_SEGMENT   equ 0B800h
INIT_LEN	equ 08h
INIT_WIDTH	equ 020h
INIT_HEIGHT	equ 0Ah

BLOCK_WIDTH	equ 160d
MAX_LEN		equ 35d

CL_BLOCK 	equ 070h
CL_SHADOW	equ 050h

;
; Destroy: ah, dx, cx
;
.wait		macro
		nop
		
  		mov ah, 086h
  		xor dx, dx
  		mov cx, 01h
  		int 15h

  		nop
  		endm

;
; Destroy: AX
;
.exit_DOS	macro
		nop

		xor al, al
		mov ah, 04ch			; stop
		int 21h				; program					

		nop
		endm


;--------------------------------------------------------------
;
; 9 interruption catcher
;
;--------------------------------------------------------------
New09		proc
		push ax di es dx

		mov di, VIDEO_SEGMENT	; di = B800
		mov es, di		; es = di
		mov di, (5*80+40)*2	; di = screen_center
		
		in al, 60h		; al = key_number
		mov ah, 04Eh		; ah = color

                mov dh, CL_SHADOW
                add dh, al
                call ProcChangeShadow	
             
Continue:	pop dx es di ax            

		db 0eah                 ; chain to the hardware interrupt
old09		dw 0                    ;
		dw 0                    ;
		
		endp
;--------------------------------------------------------------




;-----------------------------------------------------------------
; Calculate BX procedure
;
; Expect: bl = width
; 	  bh = height
; Destroy: AX
; Return:  DI
;-----------------------------------------------------------------
ProcCalcBX      proc
		cmp bh, 25d		; if ( bh < 25 )
		jl @@Next1	        ;    continue
		mov bh, 25d             ; bh = 25d
@@Next1:	cmp bl, 80d             ; if ( bl < 80 )
		jl @@Next2              ; continue
		mov bl, 80d             ; bl = 80
@@Next2:	
		mov ax, 2d		; ax = 2
		mul bl			; ax = ax * width
		mov di, ax		; di = ax                             		
		mov ax, BLOCK_WIDTH	; ax = 160	
		mul bh			; ax = ax * height
		add di, ax		; di += ax
		ret			; return di
		endp
;-----------------------------------------------------------------




;-----------------------------------------------------------------
; Draw a horizontal line
;
; Expects: di - where are from to draw
;	   al - first symbol of the line
;	   ah - background color
;	   dh - middle symbol of the line
;	   dl - last symbol of the line
;	   si - shadow color. 0, if shadow not need
;	   sp:[bp + 2] - len of the line
; Destroy: CX, AX, DI
;-----------------------------------------------------------------
ProcDrawLine  	proc
		cmp di, (80 * 25) * 2	; if ( di > screen_size)
		jl @@Next
		mov di, (80 * 25) * 2	;    di = screen_size

@@Next:		push bp
		mov bp, sp
		mov cx, [bp + 4]	; cx = len
		pop bp

		stosw			; draw a corner
		sub cx, 2		; cx -= 2
		mov al, dh		; al = dh
		rep stosw		; draw middle line
		mov al, dl		; al = dl
		stosw			; draw a corner
		
		cmp si, 0               ; if (si == 0)
		jne @@DrawShadow        ;    return
		ret
		
@@DrawShadow:	mov ax, si
		mov ah, 020h
		inc di                  ; di++
		stosw                   ; shadow
		stosw                   ; shadow
		ret
		endp
;-----------------------------------------------------------------

CL_BLOCK 	equ 070h
CL_SHADOW	equ 050h

;-----------------------------------------------------------------
; Change status of shadow
;
; Expect: dh - shadow color
;----------------------------------------------------------------- 
ProcChangeShadow  	proc

;----------------------------------------------------------------- 
; THE DO_NOT_TOUCH AREA
; 
; Do NOT Touch!!!
;----------------------------------------------------------------- 
		
		mov bl, INIT_WIDTH + INIT_LEN + ((MAX_LEN - INIT_LEN) / 4) * 3
		mov bh, INIT_HEIGHT - (MAX_LEN - INIT_LEN) / 4 + 1

		call ProcCalcBX
		mov cx, (((MAX_LEN - INIT_LEN) / 4) * 4 + INIT_LEN) / 2 + 1
	
;----------------------------------------------------------------- 
; End of DO_NOT_TOUCH Area
;
; But still do NOT touch.
;----------------------------------------------------------------- 

Shadow1:        mov al, 20h		; whitespace
		mov ah, dh		; color
		stosw	                ; draw shadow
                stosw                   ; draw shadow
                inc bh                  ; bh++
                call ProcCalcBX         ; 
                loop Shadow1            ; while (--cx)


		mov bl, INIT_WIDTH - (MAX_LEN - INIT_LEN) / 4 + 2
		call ProcCalcBX
		mov cx, (((MAX_LEN - INIT_LEN) / 4) * 4 + INIT_LEN)
	        mov ah, dh		; color
	        mov al, 20h             ; whitespace
	        rep stosw
               

		ret
		endp
;-----------------------------------------------------------------

Program:	mov bx, VIDEO_SEGMENT          
		mov es, bx 			; es = B800

		push INIT_LEN
		mov bx, (INIT_HEIGHT shl 8) or INIT_WIDTH

		
Loop_new_block:	call ProcCalcBX
                                                           
		mov al, 0C9h			; al = corner1
		mov ah, CL_BLOCK                ; ah = color
		mov dh, 0CDh                    ; dh = middle
		mov dl, 0BBh                    ; dl = corner2
		call ProcDrawLine

                inc bh                          ; height++
		call ProcCalcBX
	
		mov dl, 0BAh			; dl = corner2
		mov dh, 020h                    ; dh = middle
		mov si, CL_SHADOW		; si = shadow
		
		pop bp
		push bp
Loop_body:	cmp bp, 0                       ; if (bp == 0)
		je Stop_body                    ;    break
		sub bp, 2                       ; bp -= 2
       		
       		mov al, 0BAh                    ; al = corner1	
	  	mov ah, CL_BLOCK		; ah = color
	  	call ProcDrawLine
      	  	
      	  	inc bh                       	; bh++
		call ProcCalcBX	  	        
	  	jmp Loop_body                   ; continue
Stop_body:

		mov al, 0C8h			; al = corner1
		mov ah, CL_BLOCK                ; ah = color
		mov dh, 0CDh                    ; dh = middle
		mov dl, 0BCh                    ; dl = corner2
		call ProcDrawLine
		    
		inc bh                          ; bh++	
		call ProcCalcBX

		add di, 4 			; di += 4
		mov al, 020h			; al = corner1
		mov ah, CL_SHADOW               ; ah = color
		mov dh, 020h                    ; dh = middle
		mov dl, 020h                    ; dl = corner2
		xor si, si			; si = 0
		call ProcDrawLine
		
		; Enhancing block
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		dec bl				; width--                ;
		pop cx				; len                    ;
		shr cx, 1                       ; cx >> 1                ;
		sub bh, cl			; height--               ;
		shl cx, 1                       ; cx << 1                ;
		sub bh, 3                       ; bh -= 3                ;
		add cx, 4			; len += 4               ; 
		push cx	                                                 ;
	        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		cmp cx, MAX_LEN			; if height < MAX_LEN
		jg Exit				;   stop
						; else
		.wait				;   wait
		jmp Loop_new_block		; continue
	 			

Exit:           
		xor ax, ax				; ax = 0
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
	
TheEND:		end		Start
