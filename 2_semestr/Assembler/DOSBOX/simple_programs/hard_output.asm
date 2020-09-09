;-----------------------------------------------------
;
; Program print message in the stdout using "int 10h"
;
;-----------------------------------------------------

.model tiny				
.code
org 100h

Start:		mov di, offset Msg	; dx = &Msg
		mov ah, 0eh		; ah = 0eh
Next:		mov al, [di]		; al = &dx
		cmp al, '$'
		je EEnd
		int 10h			; print(al)
		inc di			; dx++
		jmp Next

EEnd:		mov ax, 4c00h		; exit(al = 0)
		int 21h

CR		equ 13
LF		equ 10
CRLF		equ CR, LF

Msg:		db 'Hello', CRLF, '$'
end		Start
