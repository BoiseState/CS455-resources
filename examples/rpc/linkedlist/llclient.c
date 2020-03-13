/* RPC client for simple addition example */

#include <stdio.h>
#include <stdlib.h>

#include "ll.h"  /* Created for us by rpcgen - has everything we need ! */


void printnums( node *f) 
{
  
  	while (f) {
    	printf("%d ",f->x);
    	f=f->next;
  	}
  	printf("\n");
}

void print_sum( node *head, CLIENT *clnt) 
{
  	int *result;

  	result = sum_1(head, clnt);

  	if (result == (int *) NULL) {
    	clnt_perror(clnt, "call failed");
    	return;
  	} 
    
  	printf("Sum is %d\n",*result); 
}



int main( int argc, char *argv[]) 
{
  	CLIENT *clnt;
  	int n,i;
  	node *f;
  	node *head;
  	node *prev;

  	if (argc<3) {
    	fprintf(stderr,"Usage: %s hostname num1 num2 ...\n",argv[0]);
    	exit(0);
  	}



  	clnt = clnt_create(argv[1], LL_PROG, LL_VERSION, "tcp"); 

  	/* Make sure the create worked */
  	if (clnt == (CLIENT *) NULL) {
    	clnt_pcreateerror(argv[1]);
    	exit(1);
  	}

  	/* Create a  linked list */
  	n = argc-2;
  	f = head = (node *) malloc(sizeof(node));
  	for (i=0;i<n-1;i++) {
    	f->x = atoi(argv[i+2]);
    	f->next = (node *) malloc(sizeof(node));
    	prev=f;
    	f = f->next;
  	}
  	f->x = atoi(argv[argc-1]);
  	f->next =  NULL;

  	printnums(head);

   	/* calls sum function on server */
	print_sum(head,clnt);

  return(0);
}



/* vim: set ts=4: */
