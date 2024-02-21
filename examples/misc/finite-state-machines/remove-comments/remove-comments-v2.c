
#include <stdio.h>

/**
 *
 * Remove comments from a valid C program. Comments don't nest. The program will take care 
 * to avoid embedded comments inside quotes, escape quotes inside strings (e,g, "   \" ") 
 * and character literals such as '\'' and '"'.
 *
 * gcc -Wall -o remove-blanks remove-blanks.c
 */
#define PROGRAM 0
#define START_COMMENT 1
#define END_COMMENT 2
#define INSIDE_COMMENT 3
#define QUOTE 4
#define ESCAPE_QUOTE 5
#define START_CHAR_LITERAL 6
#define END_CHAR_LITERAL 7
#define ESCAPE_LITERAL 8
#define END -1

int state = PROGRAM;

void handle_program(char c) 
{
	if (c == '/') {
		state = START_COMMENT;
		/* hold off on printing */
	} else if (c == '\'') {
		state = START_CHAR_LITERAL;
		putchar(c);
	} else if (c == '"') {
		state = QUOTE;
		putchar(c);
	} else {
		putchar(c); /* stay in same state */
	}
}

void handle_start_comment(char c) 
{
	if (c == '*') {
		state = INSIDE_COMMENT;
	} else {
		state = PROGRAM;
		putchar(c);
	}
}

void handle_inside_comment(char c)
{
	if (c == '*') {
		state = END_COMMENT;
	} else {
		/* don't print and don't change state */
	}
}

void handle_end_comment(char c) 
{
	if (c == '/') {
		state = PROGRAM;
	} else if (c == '*') {
		state = END_COMMENT;
	} else {
		state = INSIDE_COMMENT;
	}
}

void handle_quote(char c)
{
	if (c == '"') {
		state = PROGRAM;
		putchar(c);
	} else if (c == '\\') {
		state = ESCAPE_QUOTE;
		putchar(c);
	} else {
		putchar(c);
	}
}

void handle_escape_quote(char c)
{
	putchar(c);
	state = QUOTE;
}

void handle_start_char_literal(char c)
{
	if (c == '\\') {
		state = ESCAPE_LITERAL;
		putchar(c);
	} else {
		putchar(c);
		state = END_CHAR_LITERAL;
	}
}

void handle_end_char_literal(char c)
{
	if (c == '\'') {
		putchar(c);
		state = PROGRAM;
	} else {
		/* error, invalid C program  */
	}
}

void handle_escape_literal(char c)
{
	putchar(c);
	state = END_CHAR_LITERAL;
}



int main(int argc, char *argv[])
{
    int c;  

    c = getchar();
    while (c != EOF) {
		if (state == PROGRAM) {
			handle_program(c);
		} else if (state == START_COMMENT) {
			handle_start_comment(c);
		} else if (state == INSIDE_COMMENT) {
			handle_inside_comment(c);
		} else if (state == END_COMMENT) {
			handle_end_comment(c);
		} else if (state == QUOTE) {
			handle_quote(c);
		} else if (state == ESCAPE_QUOTE) {
			handle_escape_quote(c);
		} else if (state == START_CHAR_LITERAL) {
			handle_start_char_literal(c);
		} else if (state == END_CHAR_LITERAL) {
			handle_end_char_literal(c);
		} else if (state == ESCAPE_LITERAL) {
			handle_escape_literal(c);
		}

        c = getchar();
    }
    return 0;
}

