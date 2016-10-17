extern puts
 
section .data
    wrong: db 'Not today, son.',0
    right: db 'Well done!',0
 
section .text
global main
main:
    mov ebp, esp        ; set the frame pointer
    mov eax, 0x1 ; TODO
    mov ebx, 0x4 ; TODO
    mov ecx, 0x5        ; hardcoded
    cmp eax, ebx
    jns bad
    cmp ecx, ebx
    jb bad
    add eax, ebx
    xor eax, ecx
    jnz bad
good:
    lea eax, [right]
    push eax
    call puts
    pop eax
		ret
bad:
    lea eax, [wrong]
    push eax
    call puts
    pop eax
    ret
