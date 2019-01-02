section .data
    msg db      "hello, world!"

section .text
    global _start
_start:
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, msg
    mov     rdx, 13
    syscall
    mov    rax, 60
    mov    rdi, 0
    syscall
;step one:nasm -f elf64 -o hello.o hello.asm
;step two:ld -o hello hello.o
;
;rax - temporary register; when we call a syscal, rax must contain syscall number
;rdx - used to pass 3rd argument to functions
;rdi - used to pass 1st argument to functions
;rsi - pointer used to pass 2nd argument to functions
;
;line 7 rax=====sys_write
;line 8  0, 1 and 2 for standard input, standard output and standard error
;line 9 points to a character array
;line 10 specifies the number of bytes to be written from the file into the character array
;
;line 12 60 is a number of exit syscall

;http://0xax.blogspot.com/2014/08/say-hello-to-x64-assembly-part-1.html
