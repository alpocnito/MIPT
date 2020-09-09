.model tiny
.286
.code
org 100h    
Start:          jmp Start2
		
		
CheckFunc:	pop ax
		pop ax
		mov bx, offset pass
Next1:		mov dl, byte ptr [bx]
		cmp dl, '$'
		je Sup1
		cmp al, dl
		jne End1
		inc bx
		mov dl, byte ptr [bx]
		cmp ah, dl
		jne End1
		pop ax
		inc bx
		jmp Next1
End1:		call GiveBadMsg
Sup1:		call GiveGoodMsg
		ret




GiveGoodMsg:
		mov bx, offset GoodMsg
		mov ah, 02h
Next4:		mov dl, [bx]
		cmp dl, '$'
		je End4
		int 21h
		inc bx
		jmp Next4
End4:		call exitt
		ret




GiveBadMsg:
		mov bx, offset BadMsg
		mov ah, 02h
Next3:		mov dl, [bx]
		cmp dl, '$'
		je End3
		int 21h
		inc bx
		jmp Next3
End3:		call exitt
		ret



Hello:
		mov bx, offset HelloMsg
		mov ah, 02h
Next5:		mov dl, [bx]
		cmp dl, '$'
		je End5
		int 21h
		inc bx
		jmp Next5
End5:		ret




ReadFunc:       pop di
		mov cx, 0		; low
Next2:		mov ah, 01h
		int 21h

		cmp al, 0Dh
		je End2
		cmp cx, 0
		jne low_sit

		mov dh, al
		push dx
		inc cx
	 	jmp high_sit	
low_sit:	
		pop dx
		mov dl, al
		push dx
		dec cx
high_sit:	
		jmp Next2
End2:		push di
		ret


exitt:		
		mov ax, 4c00h
		int 21h

                           

pass:		db 'drowssap', '$$'
HelloMsg:	db 0ah, 'Hey, dude. Give me password and you will watch this film', 0ah, '$'
GoodMsg:	db 0ah, 'Congratulations, you got it', '$'
BadMsg:		db 0ah, 'Bad password bro, try again', '$'

Start2:		call Hello
		call ReadFunc
		call CheckFunc
		call exitt
      
end 		Start
	
