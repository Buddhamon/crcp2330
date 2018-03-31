// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(IOLOOP)
	@KBD // if(key)
	D=M
	@BLACK
	D;JNE // turn screen black
	@WHITE
	0;JMP // else turn screen white

(WHITE) // Change the screen color bit to white
	@color
	M=0
	@FILL 
	0;JMP

(BLACK) // Change the screen color bit to black
	@color
	M=-1
	@FILL 
	0;JMP

(FILL) // Fill in screen with screen color bit
	@SCREEN
	D=A
	@index
	M=D
	@8192
	D=A
	@index
	M=M+D

	(LOOP)
		@color
		D=M
		@index
		A=M
		M=D
		D=A

		@temp // temp = &index - &Screen
		M=D
		@SCREEN
		D=A
		@temp
		D=D-M

		@IOLOOP // if(screen filled or if temp == 0) repeat IOLOOP
		D;JEQ
		@index
		M=M-1 // index--
		@LOOP // else continue filling screen
		0;JMP




