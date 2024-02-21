#include <stdio.h>

/**
 * This program intentionally generates warnings on compilation.
 * "test 1" 'test 2' \" \' \\  test3 ** 
 test4 **/

int main() {
	/* test5 printing escape character \*/
	char *c = "'/* test6 */ \" /* test7 */\"";
	printf("%c %c \n", '\\', '\'');
	return 0;
}
