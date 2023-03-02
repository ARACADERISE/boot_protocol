HW_EQUIP_PS2     equ 4          ; PS2 mouse installed?
MOUSE_PKT_BYTES  equ 3          ; Number of bytes in mouse packet
MOUSE_RESOLUTION equ 3          ; Mouse resolution 8 counts/mm

xcoord: db "X-Coord: ", 0x0
ycoord: db "Y-Coord: ", 0x0
mouse_state: db "Mouse State: ", 0x0

mouseX_addr equ 0xC00A
mouseY_addr equ 0xC00C
mouseStatus_addr equ 0xC00E

mouseX:       dw 1              ; Current mouse X coordinate
mouseY:       dw 1              ; Current mouse Y coordinate
curStatus:    db 0              ; Current mouse status
noMouseMsg:   db "Error setting up & initializing mouse", 0x0d, 0x0a, 0
delimCommaSpc:db ", ", 0
mouse_success: db "Success on setting up & initializing mouse", 0xD, 0xA, 0x0

%include "protocol/mouse/init_mouse.s"
%include "protocol/mouse/mouse_callback.s"
%include "protocol/mouse/poll_mouse.s"

;
;   __config_mouse: back-end stub
;
;       Setup the mouse
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __config_mouse
__config_mouse:
    call __mouse_initialize
    jc .__no_mouse                ; If CF set then error, inform user and end
    call __mouse_enable           ; Enable the mouse

    jmp .__finish
.__no_mouse:
    mov si, noMouseMsg
    call __asm_print
    ret
.__finish:
    ;mov si, mouse_success
    ;call print
    ret

global get_mouse_until
get_mouse_until:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    mov cx, [ebp + 12]

    pop ebp

    call __poll_mouse

    mov ax, [mouseX]
    mov bx, [mouseY]

    cmp ax, dx
    je .end

    cmp ax, cx
    je .end

    jmp get_mouse_until
.end:
    ret

global get_mouse_until_both
get_mouse_until_both:
    call __poll_mouse

    mov dx, [mouseX_addr]
    mov bx, [mouseY_addr]

    push ebp
    mov ebp, esp

    mov ax, [ebp + 8]
    mov cx, [ebp + 12]

    pop ebp

    ; If the current x position is equal to the x value we get via the argument `x`
    ; then check y.
    cmp dx, ax
    je .check_y

    ; Redo, the above failed.
    jmp get_mouse_until_both

.check_y:
    ; Check if the y value does not equal the y value we get via the argument `y`
    cmp bx, cx
    jne get_mouse_until_both

    ; If the above fails, just return :D
    ret