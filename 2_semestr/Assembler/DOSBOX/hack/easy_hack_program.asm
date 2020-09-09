.model tiny
.286
.code
org 100h

Start:		mov ax, 0ABCDh
		push ax	
		call Hello
		call ReadFunc
		call GiveGoodMsg
		call exitt

ReadFunc:
		mov bx, offset Msg
Next1:		mov ah, 01h
		int 21h

		cmp al, 0Dh
		je End1
		mov [bx], al
		cmp byte ptr [bx+1], 024h
		je End1
		inc bx
		jmp Next1
End1:		ret


GiveBadMsg:
		mov bx, offset BadMsg
		mov ah, 02h
Next2:		mov dl, [bx]
		cmp dl, '$'
		je End2
		int 21h
		inc bx
		jmp Next2
End2:		call exitt
		ret


Hello:
		mov bx, offset HelloMsg
		mov ah, 02h
Next11:		mov dl, [bx]
		cmp dl, '$'
		je End11
		int 21h
		inc bx
		jmp Next11
End11:		ret





GiveGoodMsg:
		mov bx, offset GoodMsg
		mov ah, 02h
Next4:		mov dl, [bx]
		cmp dl, '$'
		je End4
		int 21h
		inc bx
		call f1
		call f2
		call f3
		call f4
		call f5
		call f6
		call f7
		jmp Next4
End4:		ret


f1:		
		pusha
		mov bx, offset Msg
		inc bx
		inc bx
		inc bx
		mov dl, [bx]
		cmp dl, 's'
		je End3
		call GiveBadMsg
		call exitt
End3:		popa
		ret




f2:		
		pusha
		mov bx, offset Msg
		inc bx
		inc bx
		inc bx
		inc bx
		mov dl, [bx]
		cmp dl, 'w'
		je End3
		call GiveBadMsg
		call exitt
End5:		ret





f3:		
		pusha
		mov bx, offset Msg
		inc bx
		inc bx
		mov dl, [bx]
		cmp dl, 's'
		je End3
		call GiveBadMsg
		call exitt
End6:		ret





f4:		
		pusha
		mov bx, offset Msg
		inc bx
		inc bx
		inc bx
		inc bx
		inc bx
		mov dl, [bx]
		cmp dl, 'o'
		je End3
		call GiveBadMsg
		call exitt
End7:		ret





f5:		
		pusha
		mov bx, offset Msg
		inc bx
		inc bx
		inc bx
		inc bx
		inc bx
		inc bx
		mov dl, [bx]
		cmp dl, 'r'
		je End3
		call GiveBadMsg
		call exitt
End8:		ret





f6:		
		pusha
		mov bx, offset Msg
		inc bx
		mov dl, [bx]
		cmp dl, 'a'
		je End3
		call GiveBadMsg
		call exitt
End9:		ret





f7:		
		pusha
		mov bx, offset Msg
		mov dl, [bx]
		cmp dl, 'p'
		je End3
		call GiveBadMsg
		call exitt
End10:		ret


exitt:		
		mov ax, 4c00h
		int 21h


HelloMsg	db 0ah, 'Hello boy. Write password to play SUPER-DUPER game', 0ah, '$'
GoodMsg:	db 0ah, 'Congratulations, you got it', '$'
BadMsg:		db 0ah, 'Bad password bro, try again', '$'
Msg:		db '00000000', '$'
end 		Start                                      	
