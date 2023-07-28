bits 16         ; Tell NASM this is 16 bit code
org 0x7c00      ; Tell NASM to start outputting stuff at offset 0x7c00

start:

boot:
    mov si, message     ; Point SI register to our message buffer
    call print_message  ; Call our string-printing routine    
    call get_input      ; Call our input-reading routine
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

get_input:
    ; Clear keyboard buffer by repeatedly reading it until it's empty
    mov ah, 0x01         ; Set AH register to 0x01 to specify 'int 16h' 'read keyboard buffer' function

.clear_buffer:
    int 16h              ; Invoke the interrupt to read a character from the keyboard buffer
    jz .buffer_cleared   ; If the zero flag is set, jump to buffer_cleared (keyboard buffer is empty)

    ; Keyboard buffer is not empty, discard character and try again
    mov ah, 0x00         ; Set AH register to 0x00 to specify 'int 16h' 'read keyboard buffer without echo' function
    int 16h              ; Invoke the interrupt to read a character from the keyboard buffer
    jmp .clear_buffer    ; Jump back to the beginning of the loop to clear the keyboard buffer again

.buffer_cleared:
    ; Initialize input buffer
    xor di, di          ; Clear DI register
    mov di, input       ; Initialize DI register to point to the beginning of the input buffer
    mov cx, 16          ; Initialize CX register to the maximum input buffer size

.repeat_get:
    ; Read a character from the keyboard
    mov ah, 0h          ; Specify 'int 16h' 'keyboard input' function
    int 16h             ; Invoke the interrupt to read a character

    ; Store the input character in the buffer
    cmp al, 0Dh         ; Check if the Enter key has been pressed
    je .end_get         ; If it has, jump to end_read
    mov byte [di], al   ; Store the input character in memory
    inc di              ; Increment DI register to move to the next byte in the input buffer
    dec cx              ; Decrement CX register to track the remaining buffer size
    jz .end_get         ; If the buffer is full, jump to end_read
    jmp .repeat_get     ; Jump back to the beginning of the loop to read the next character

.end_get:
    mov byte [di], 0    ; Store a null byte at the end of the input buffer
    ret                 ; Return to the caller

print_greeting:
    mov si, greeting    ; Point SI register to our greeting buffer
    call print_message  ; Call our string-printing routine
    mov si, input       ; Point SI register to the input buffer
    call print_message  ; Call our string-printing routine to print the user's name
    ret                 ; Return to the caller

data:
    message db 'What is your name?', 10, 13, 0
    ; Buffer to store user input
    input times 16 db 0
    greeting db 'Hello, ', 0

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!
