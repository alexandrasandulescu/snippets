extern printf
section .data
	format: db '%d', 10, 0
section .text
global main
main:
    ;cele doua multimi se gasesc în eax si ebx
    mov eax, 139 
    mov ebx, 169
    ;PRINT_DEC 4, eax ; afiseaza prima multime
    ;NEWLINE
    ;PRINT_DEC 4, ebx ; afiseaza cea de-a doua multime
    ;NEWLINE
    
   ; TODO1: reuniunea a doua multimi
	  xor esi, esi ; add result in esi
		mov ecx, 0
		jmp while1
cond1:
  mov edx, eax
	shr edx, cl
  shl edx, cl
  or esi, edx
  mov edx, ebx
	shr edx, cl
  shl edx, cl
  or esi, edx

	;mov edx, eax
	;shr edx, cl
  ;shl edx, cl
	;mov edi, edx
  ;mov edx, ebx
	;shr edx, cl
  ;shl edx, cl
  ;and edi, edx
  ;or esi, edi
	;inc ecx
	inc ecx
while1:
	cmp ecx, 0x20 ;counter
	jl cond1

	push esi
	push dword format
	call printf
	pop eax
	pop eax


    
    ; TODO2: adaugarea unui element în multime

    
    ; TODO3: intersectia a doua multimi

    
    ; TODO4: complementul unei multimi

    
    ; TODO5: eliminrea unui element

    
    ; TODO6: diferenta de multimi EAX-EBX

    
    xor eax, eax
    ret
