.section .data
    hello:
        .string "hello"

.section .text
.globl exercise2
.type exercise2, @function
exercise2:

    li t0, 1
    excute:
        li a0, 5
        li a1, 5
        li a2, 35
        li a3, 35
        li a4, 0xFFE0
        jal LCD_DrawRectangle

        li a0, 60
        li a1, 20
        li a2, 15
        li a3, 0x07E0
        jal LCD_DrawCircle

        li a0, 50
        li a1, 50
        la a2, hello
        li a3, 0xF800
        jal LCD_ShowString

        li a0, 0x0000
        jal LCD_Clear

    nothing:
        addi x0, x0, 0

    jal Get_BOOT0
    beq a0, t0, excute
    jal nothing

.end exercise2
