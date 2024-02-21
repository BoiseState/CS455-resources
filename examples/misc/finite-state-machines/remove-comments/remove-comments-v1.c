
#include <stdio.h>

/**
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

int main(int argc, char *argv[])
{
    int c;  
	int state = PROGRAM;

    c = getchar();
    while (c != EOF) {
		if (state == PROGRAM) {
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
		} else if (state == START_COMMENT) {
			if (c == '*') {
				state = INSIDE_COMMENT;
			} else {
				state = PROGRAM;
				putchar(c);
			}
		} else if (state == INSIDE_COMMENT) {
			if (c == '*') {
				state = END_COMMENT;
			} else {
				/* don't print and don't change state */
			}
		} else if (state == END_COMMENT) {
			if (c == '/') {
				state = PROGRAM;
			} else if (c == '*') {
				state = END_COMMENT;
			} else {
				state = INSIDE_COMMENT;
			}
		} else if (state == QUOTE) {
			if (c == '"') {
				state = PROGRAM;
				putchar(c);
			} else if (c == '\\') {
				state = ESCAPE_QUOTE;
				putchar(c);
			} else {
				putchar(c);
			}
		} else if (state == ESCAPE_QUOTE) {
			putchar(c);
			state = QUOTE;
		} else if (state == START_CHAR_LITERAL) {
			if (c == '\\') {
				state = ESCAPE_LITERAL;
				putchar(c);
			} else {
				putchar(c);
				state = END_CHAR_LITERAL;
			}
		} else if (state == END_CHAR_LITERAL) {
			if (c == '\'') {
				putchar(c);
				state = PROGRAM;
			} else {
				/* error, invalid C program  */
			}
		} else if (state == ESCAPE_LITERAL) {
			putchar(c);
			state = END_CHAR_LITERAL;
		}

        c = getchar();
    }
    return 0;
}


