use16
global print_str

print_str:
	mov ah, 0x02
	mov dh, 0x02
	mov dl, 0x00
	mov bh, 0x00
	int 0x10

    push ebp
    mov ebp, esp

    mov si, [ebp + 8]

    pop ebp
	mov ah, 0x0e
.print:
	mov al, [si]
	cmp al, 0x0
	je .end_print
	int 0x10

	cmp al, 0xA
	je .reset
	jmp .cont
.reset:
	mov ah, 0x0E
	mov al, 0x0D
	int 0x10
.cont:
	
	add si, 1

	jmp .print

.end_print:
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
	ret

global print

print:
	mov ah, 0x0e
.print:
	mov al, [si]
	cmp al, 0x0
	je .end_print
	int 0x10
	
	add si, 1

	jmp .print

.end_print:
    mov dl, 0x0
    mov dh, 0x1
    mov ah, 0x02
    int 0x10

	ret