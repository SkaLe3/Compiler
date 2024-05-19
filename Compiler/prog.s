.386
.model flat, C
.data
	var1 DWORD ?
	var2 DWORD ?
.code
main proc
	jmp tt1_full_grammar_example
tt1_full_grammar_example:
	mov eax, 13
debug1: 13
	mov DWORD PTR [var1], eax
debug2: var1
	mov eax, DWORD PTR [var1]
	mov ebx, 111
	cmp eax, ebx
	jne ?L0
	mov eax, DWORD PTR [var2]
debug1: 111
	mov DWORD PTR [var1], eax
debug2: var1
	jmp ?L1
?L0:
	mov eax, DWORD PTR [var2]
	mov ebx, DWORD PTR [var1]
	cmp eax, ebx
	jne ?L2
	jmp ?L3
?L2:
?L3:
	mov eax, 7
debug1: 7
	mov DWORD PTR [var2], eax
debug2: var2
?L1:
	mov eax, 222
	mov ebx, 222
	cmp eax, ebx
	jne ?L4
	mov eax, 333
debug1: 333
	mov DWORD PTR [var2], eax
debug2: var2
	jmp ?L5
?L4:
?L5:
main endp
end main
