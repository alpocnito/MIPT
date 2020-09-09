;-----------------------------------------------------
;
; Program read string from stdin, which is end with $
;	and print it in stdout, BUT no more than 
;	40h symbols
;
;-----------------------------------------------------

.model tiny
.code
org 100h
locals @@

LF equ 0ah
CR equ 0dh


Start: 	call main

;--------------------------------------
; MACROS	EXITDOS
; ENTRY:	NONE
; EXIT:		DOS
;--------------------------------------
.exitDOS 	macro
	mov ax, 4c00h
	int 21h
	endm

;--------------------------------------
; FUNCTION  MAIN
; ENTRY:    NONE
; EXIT:     NONE
;--------------------------------------
main    proc
    	mov bx, offset string
	mov cx, 40h
	call getSTRING

; printf("\n")

	mov al, LF
	call putCHAR
	mov al, CR
	call putCHAR

; printf("\n")

	mov bx, offset string
	mov cx, 40h
	call putSTRING
    	.exitDOS
    	endp

;--------------------------------------
; FUNCTION	PRINTCHAR
; ENTRY:	AL - CHAR TO WRITE
; EXIT:		NONE
;--------------------------------------
putCHAR 	proc 
	;mov ah, 0ah
	mov ah, 0eh
	mov bh, 0h
	mov cx, 1
	int 10h
	ret
	endp

;--------------------------------------
; FUNCTION 	PRINTSTRING
; ENTRY: 	CX - STRING LENGTH
;		BX - STRING OFFSET
; EXIT:		NONE
;--------------------------------------
putSTRING 	proc
	cmp cx, 0
	je @@next
@@for:	
	mov al, byte ptr ds:[bx]
	inc bx
	cmp al, '$'
	je @@next
	mov di, bx
	mov si, cx
	call putCHAR
	mov cx, si
	mov bx, di
	dec cx
	jnz @@for
@@next: 
	ret
	endp

;--------------------------------------
; FUNCTION 	GETCHAR
; ENTRY: 	NONE
; EXIT: 	AL - FETCHED CHAR
;--------------------------------------
getCHAR 	proc
	mov ah, 01h
	int 21h
	ret
	endp

;--------------------------------------
; FUNCTION	GETSTRING
; COMMENT:	'$' - END SYMBOL
; ENTRY:	CX - STRING LENGTH
;		BX - STRING OFFSET (FROM DS)
; EXIT:		NONE
;--------------------------------------
getSTRING 	proc
	cmp cx, 0
	je @@next
@@for:
	call getCHAR
	mov byte ptr ds:[bx], al
	inc bx
	cmp al, '$'
	je @@next
	dec cx
	jnz @@for
@@next:
	ret
	endp

;--------------------------------------
.data

string: db '0000', '$'

end Start
