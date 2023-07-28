bits 16         ; Tell NASM this is 16 bit code
org 0x7c00      ; Tell NASM to start outputting stuff at offset 0x7c00

start:

boot:
    mov si, prompt      ; Point SI register to our message buffer
    call print_message  ; Call our string-printing routine
    
    mov si, input	 ; 
    call get_input	 ;
    
    mov si, greeting	 ;
    call print_message	 ;
    
    mov si, input 	 ;
    call print_message	 ;
    
print_message:
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
                        ; [AL = Character, BH = Page Number, BL = Colour (in graphics mode)]

.repeat_print:
    lodsb               ; Load byte at address SI into AL, and increment SI
    cmp al, 0           ; Check if we've reached the end of the message
    je .end_print       ; If we have, jump to end_print
    int 10h             ; Invoke the interupt to print out the character
    jmp .repeat_print   ; Jump back to the beginning of the loop to print the next character

.end_print:
    cli                 ; Clear interrupt flag
    hlt                 ; Halt execution
    
get_input:
    mov di, input      ; Point DI to the start of our input buffer
    mov cx, 0           ; Initialize counter to zero
    
.repeat_get:
    mov ah, 0h          ; Read a keypress from the keyboard
    int 16h
    cmp al, 8           ; Check if Backspace was pressed
    je .handle_backspace
    cmp al, 13          ; Check if Enter was pressed
    je .end_get
    mov [di], al        ; Store the user's input character in our buffer
    inc di              ; Increment the buffer pointer
    inc cx              ; Increment the counter
    mov ah, 0Eh         ; Echo the character back to the screen
    int 10h
    jmp .repeat_get
    
.handle_backspace:
    cmp cx, 0           ; Check if the buffer is already empty
    je .repeat_get      ; If it is, jump back to the input loop
    dec di              ; Decrement the buffer pointer
    dec cx              ; Decrement the counter
    mov al, ' '         ; Output a space to clear the character on the screen
    mov ah, 0Eh
    int 10h
    mov al, 8           ; Output a Backspace to move the cursor back
    mov ah, 0Eh
    int 10h
    jmp .repeat_get
    
.end_get:
    mov byte [di], 0    ; Null-terminate the buffer
    ret                 ; Return to the calling function

data:
    prompt db 'What is your name?', 10, 13, 0
    greeting db 'Hello, ', 0
    input db 30, 0

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!
