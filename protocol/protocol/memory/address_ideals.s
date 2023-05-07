;
;   __test_address: back-end stub
;
;       I don't know yet
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __test_address
something: dw 0x0
__test_address:
    push ebp
    mov ebp, esp

    ; The address
    mov si, [ebp + 8]

    lodsb

    cmp al, 0x0
    je .yes
    jmp .no
.yes:
    mov ah, 0x0
    jmp .end
.yes_hlt:
    jmp .yes_hlt
.no:
    mov ah, 0x01
.end:
    pop ebp
    ret