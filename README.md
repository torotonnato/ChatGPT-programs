# ChatGPT made programs

## AST.go

		Write a golang function that accept a string s as a parameter. The string has this grammar:

		program := (stmt delim)+
		delim := ';' '\n+'
		stmt := op reg reg
		op := 'add' | 'sub' | 'mul' | 'div'
		reg := 'ax' | 'bx' | 'cx' | 'dx'

		The output of the function is an AST
		
Note: delim (line #4) should have been "delim := ';' '\n'+"
The program is correct, but doesn't take into consideration newlines as delimiters between
statements. No checks for op or reg validity.

Nonetheless impressive.

## oisc.c
		Write a small C function that accept an array "disp" of integers
		and returns an array of bytes. The function will output valid x86
		instructions that codify a jump-if-not-zero to each value in "disp"

A warning from the compiler 

		oisc.c:39:9: warning: incompatible implicit declaration of built-in function ‘memcpy’
		oisc.c:9:1: note: include ‘<string.h>’ or provide a declaration of ‘memcpy’

and a minor (but fatal) logic error at lines 38 and 39:

		int jnz_size = sizeof(jnz);
		memcpy(buf + offset, jnz, jnz_size);

Still impressed!

## pong.c

		Write a small C program to play pong on the terminal using ansi escape characters

No compile errors, added a small function to put the terminal in non-canonical mode.
w, s (player1) and i, k (player2) to play.
Note: ^C to quit, reset the terminal manually.

# Unbelievable
