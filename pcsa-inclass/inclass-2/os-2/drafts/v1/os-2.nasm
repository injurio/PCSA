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
    xor cx, cx          ; Clear CX register to use as counter
    
.repeat_get:
    mov ah, 0h          ; Specify 'int 16h' 'read key press' function
    int 16h	     	 ;
    cmp al, 0x0D        ; Check if the user pressed enter (0x0D = carriage return)
    je .end_get         ; If they have, jump to end_get
    cmp al, 0x08        ; Check if the user pressed backspace (0x08 = backspace)
    je .backspace       ; If they have, jump to backspace
    mov [si+cx], al     ; Store the user's input character in our buffer
    inc cx              ; Increment CX register to move to the next character in the buffer
    jmp .repeat_get     ; Jump back to the beginning of the loop to get the next key press
    
.backspace:
    cmp cx, 0           ; Check if the buffer is empty
    je .repeat_get      ; If it is, jump back to the beginning of the loop to get the next key press
    dec cx              ; Decrement CX register to move back to the previous character in the buffer
    mov byte [si+cx], 0 ; Clear the character that we just moved back from
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
    mov al, 0x08        ; Move the cursor back one position
    int 10h             ; Invoke the interrupt to move the cursor back
    mov al, ' '         ; Print a space to clear the character that we just moved back from
    int 10h             ; Invoke the interrupt to print the space
    mov al, 0x08        ; Move the cursor back one position again
    int 10h             ; Invoke the interrupt to move the cursor back
    jmp .repeat_get	 ;
    
.end_get:
    cli                 ; Clear interrupt flag
    hlt                 ; Halt execution

data:
    prompt db 'What is your name?', 10, 13, 0
    greeting db 'Hello, ', 0
    input db 30, 0

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!
