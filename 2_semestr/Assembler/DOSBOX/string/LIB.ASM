;---------------------------------------------------------
; Sets the first NUM bytes of the block of memory 
;  pointed by PTR to the specified VALUE
; 
; Expect: cx - NUM
;	  al - VALUE
;	  es:[di] - PTR
;
;---------------------------------------------------------
memset 		proc
		cld			; di raises
		jcxz end_memset		; end, if cx == 0

		rep stosb

end_memset:	ret
		endp
;--------------------------------------------------------



;---------------------------------------------------------
; Searches within the first NUM bytes of the block of 
;  memory pointed by PTR for the first occurrence of 
;  VALUE, and returns a pointer to it, relatively to ES
; 
; Expect: cx - NUM
;	  al - VALUE
;	  es:[di] - PTR
;
; Return: DI
;  If the value is not found, 
;  the function returns a null pointer
;---------------------------------------------------------
memchr 		proc
		cld			; di raises
		jcxz end_memchr		; end, if cx == 0

		repne scasb  		; find al
		jne memchr_failed       ; if failed
		dec di                  ;    di--
		ret                     ;    return
memchr_failed:  mov di, 0               ; else
		
end_memchr:	ret                     ;    return 0	
		endp
;--------------------------------------------------------




;---------------------------------------------------------
; Copies the values of NUM bytes from the location pointed 
;  to by SOURCE directly to the memory block pointed to 
;  by DESTINATION
; 
; Expect: cx - NUM
;	  es:[si] - SOURCE
;	  es:[di] - DESTINATION
;
; Return: DI
;  If the value is not found, 
;  the function returns a null pointer
;---------------------------------------------------------
memcpy 		proc
		cld			; di raises
		jcxz end_memcpy		; end, if cx == 0

		rep movsb

end_memcpy:	ret	
		endp
;--------------------------------------------------------




;---------------------------------------------------------
; Compares the first NUM bytes of the block of memory 
;  pointed by PTR1 to the first num bytes pointed by PTR2, 
;  returning zero if they all match or a value different 
;  from zero representing which is greater if they do not 
; 
; Expect: cx - NUM
;	  es:[si] - PTR1
;	  es:[di] - PTR2
;
; Return: CX < 0  the first byte that does not match in both 
;		   memory blocks has a lower value in ptr1 than in ptr2 
;	  CX = 0  the contents of both memory blocks are equal
;	  CX > 0  the first byte that does not match in both 
;		  memory blocks has a greater value in ptr1 than in ptr2 
;          
;---------------------------------------------------------
memcmp 		proc
		cld			; di raises
		jcxz end_memcmp		; end, if cx == 0

		repe cmpsb
		
		je memcmp_equal
		jg memcmp_greater
		mov cx, -40
		ret
memcmp_equal:   mov cx, 0
		ret
memcmp_greater: mov cx, 40
		
end_memcmp:	ret	
		endp
;--------------------------------------------------------



;---------------------------------------------------------
; Returns the length of the C string STR
;
; Expect: es:[di] - STR
; Destroy: AL, DI
; Return: SI - len of the string
;---------------------------------------------------------
strlen 		proc
		cld			; di raises
		mov al, 0		; '\0'
		mov si, di		; save di

		repne scasb  
		sub di, si
		mov si, di
		dec si

		ret	
		endp
;--------------------------------------------------------



;---------------------------------------------------------
; Returns a pointer to the first occurrence of CHAR 
;  in the C string STR 
; 
; Expect: al - CHAR
;	  es:[di] - STR
;
; Return: DI - a pointer to the first occurrence of character in str
;
; If the character is not found, the function returns a null pointer
;---------------------------------------------------------
strchr 		proc

strchr_loop:	cmp es:[di], al         ; if (es:[di] == al)
		je strchr_found         ;    break
		cmp byte ptr es:[di], 0 ; if (es:[di] == '\0')
		je strchr_failed        ;    break
		inc di                  ; di++
		jmp strchr_loop		; goto loop

strchr_found:   ret
strchr_failed:  mov di, 0
		ret           	
		endp
;--------------------------------------------------------



;---------------------------------------------------------
; Returns a pointer to the last occurrence of CHAR 
;  in the C string STR 
; 
; Expect: al - CHAR
;	  es:[di] - STR
;
; Return: SI - a pointer to the last occurrence of character in str
;
; If the character is not found, the function returns a null pointer
;---------------------------------------------------------
strrchr 	proc
		xor si, si		        	

strrchr_loop:	cmp es:[di], al         ; if (es:[di] == al)
		je strrchr_save_di      ;    si = di
		cmp byte ptr es:[di], 0 ; if (es:[di] == '\0')
		je strrchr_end          ;    break
strrchr_cont:   inc di                  ; di++
		jmp strrchr_loop	; goto loop

strrchr_end:	ret

strrchr_save_di:mov si, di
		jmp strrchr_cont           	
		endp
;--------------------------------------------------------                      9




;---------------------------------------------------------
; Copies the C string pointed by SOURCE into the array 
;  pointed by DEST, including the terminating null character 
;  (and stopping at that point) 
; Expect: es:[si] - SOURCE
;	  es:[di] - DESTINATION
; Destroy: AL
;---------------------------------------------------------
strcpy 		proc

strcpy_loop:	mov al, es:[si]		; di = si 
		mov es:[di], al         ;
		cmp byte ptr es:[si], 0	; if (es:[si] == '\0')
		je end_strcpy		;    break
		inc si
		inc di
		jmp strcpy_loop

end_strcpy:	ret	
		endp
;--------------------------------------------------------

                                                                                          
b_p EQU byte ptr
;---------------------------------------------------------
; Returns a pointer to the first occurrence of STR2 in STR1, 
;  or a null pointer if str2 is not part of str1.
;
; The matching process does not include the terminating 
;  null-characters, but it stops there.
;
; Expect: es:[si] - STR1
;	  es:[di] - STR2
; Destroy: CX, BX, DX
; Return: AX
;---------------------------------------------------------
strstr 		proc
		mov bx, di		; save di
		xor ax, ax		; clear ax

strstr_cont:	cmp b_p es:[si], 0 	; if (si == 0)
		je end_strstr           ;    end
		mov dl, es:[di]
		mov dh, es:[si]
		cmp dl, dh		; if (si == di)
		je strstr_inner     	;    inner_loop
		inc si                 	; si++
		jmp strstr_cont         ; continue

strstr_inner:	mov cx, si		; save cx
		inc cx
strstr_inner_l:	inc di
		inc si
		cmp b_p es:[di], 0      ; if (di == 0)
		je strstr_found         ;    found
		cmp b_p es:[si], 0      ; if (si == 0)
		je end_strstr           ;    end
		mov dl, es:[di]
		mov dh, es:[si]
		cmp dl, dh		; if (si == di)
		je strstr_inner_l       ; continue
		mov di, bx		; restore di
		mov si, cx              ; restore si
		jmp strstr_cont

strstr_found:   mov ax, cx
		dec ax
		jmp end_strstr

end_strstr:	ret	
		endp
;--------------------------------------------------------
