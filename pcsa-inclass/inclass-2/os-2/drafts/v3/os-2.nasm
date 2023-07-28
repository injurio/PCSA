bits 16         ; Tell NASM this is 16 bit code
org 0x7c00      ; Tell NASM to start outputting stuff at offset 0x7c00

start:

boot:
    mov si, message     ; Point SI register to our message buffer
    call print_message  ; Call our string-printing routine    
    call read_input     ; Call our input-reading routine
    call print_greeting ; Call our greeting-printing routine

print_message:
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function

.repeat_print:
    lodsb               ; Load byte at address SI into AL, and increment SI
    cmp al, 0           ; Check if we've reached the end of the message
    je .end_print       ; If we have, jump to end_print
    int 10h             ; Invoke the interupt to print out the character
    jmp .repeat_print   ; Jump back to the beginning of the loop to print the next character

.end_print:
    ret                 ; Return to the caller

read_input:
    xor ax, ax          ; Clear AX register
    mov ah, 0           ; Specify 'int 16h' 'keyboard input' function

.repeat_read:
    int 16h             ; Invoke the interrupt to read a character
    cmp al, 0Dh         ; Check if the Enter key has been pressed
    je .end_read        ; If it has, jump to end_read
    mov bl, al          ; Move the input character into BL register
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
    int 10h             ; Invoke the interrupt to print the input character
    mov [input], bl     ; Store the input character in memory
    inc di              ; Increment DI register to move to the next byte in the input buffer
    jmp .repeat_read    ; Jump back to the beginning of the loop to read the next character

.end_read:
    ret                 ; Return to the caller

print_greeting:
    mov si, greeting    ; Point SI register to our greeting buffer
    call print_message  ; Call our string-printing routine
    ret                 ; Return to the caller

data:
    message db 'What is your name?', 10, 13, 0
    input times 16 db 0 ; Buffer to store user input
    greeting db 'Hello, ', 0

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!

