#include <stdio.h>

/*
 * This program intentionally generates warnings on compilation.
 * "test 1" 'test 2' \" \' \\
 */

int main() {
	/* test3 printing escape character \*/
	char *c = "'/* test4 */ \" /* test5 */\"";
	printf("%c %c \n", '\\', '\'');
	return 0;
}
