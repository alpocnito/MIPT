;-----------------------------------------------------
;
; Program print message in the stdout using "int 21h"
;
;-----------------------------------------------------

.model tiny				
.code
org 100h

Start:		mov bx, offset Msg	;
		mov ah, 02h		; ah = 2
Next:		mov dl, [bx]		; al = &bx
		cmp dl, '$'
		je End
		int 21h			;
		inc bx			; bx++
		jmp Next

End:		mov ax, 4c00h		; exit(al = 0)
		int 21h

CR		equ 13
LF		equ 10
CRLF		equ CR, LF

Msg:		db 'Hello', CRLF, '$'

end		Start
