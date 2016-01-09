
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *password;
	char *blanks;
	int length;
	int i;

	length = sysconf(_SC_PASS_MAX);
	blanks = (char *) malloc(sizeof(char)*length);
	for (i=0; i<length; i++)
		blanks[i] = ' ';

	password = getpass("Enter your password:");
	printf("\nPassword entered: %s\n", password);
	// clear the password as soon as it has been used
	memcpy(password, blanks, strlen(password));
	exit(0);
}
