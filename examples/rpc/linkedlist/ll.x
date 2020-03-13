/* Linked list example - a linked list of integers,
   The remote procedure returns the sum
*/
#define VERSION_NUMBER 1

struct node {
	int x;
	node *next;
};


program LL_PROG {
   version LL_VERSION {
     int SUM(node) = 1;
   } = VERSION_NUMBER;
} = 555553555;

