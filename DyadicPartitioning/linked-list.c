#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "linked-list.h"

/*structure for the memoization node that keeps in the next node, 
* cost of splitting, and coordinates in the format lb_0, lb_1,...ln_N-1,
* ub_0, ub_1,...ub_N-1
*/
typedef struct memNode *memNode;
struct memNode{
	struct memNode *next;
	//int type;
	int cost;
	double *coordinates;
};

//structure list to keep the memoization nodes linked
struct list{
	struct memNode *head;
};

//makes a list
struct list *mkList(){
	struct list *rv = (struct list *)malloc(sizeof(*rv));
	rv->head = NULL;
	return rv;
};

//adds a mew coordinate and cost to the memoization
struct memNode *addToMem(struct memNode *next, int cost, double *coordinates){
	struct memNode *rv = (struct memNode *)malloc(sizeof(*rv));
	int n = sizeof(coordinates);
	rv->coordinates=malloc(sizeof(double)* n);
	rv->next = next; 
	rv->cost = cost;
	
	int i;
	for (i = 0; i < n; i++){
		rv->coordinates[i] = coordinates[i];
	}
	return rv;
}

//puts the new coordinate and cost into the list at the front of the list
void addToList(struct list *l, int cost, double *coordinates){
	l->head = addToMem(l->head,cost, coordinates);
}

//print test for memoization
void printMemNode(struct memNode *l){
	while (l != NULL) {
		printf("%d ", l->cost);
		int n = sizeof(l->coordinates);
		int i;
		for(i = 0; i < n; i++){
			printf("%lf ", l->coordinates[i]);
		}
		l = l->next;
		printf("\n");
	}
	printf("\n");
}

//part of the print test
void printList(struct list *l){
	printMemNode(l->head);
}

//check if coordinates is in list
int listContains(list l, double *coord){
	return memContains(l->head, coord);
}

//check if coordinate is in list
int memContains(memNode l, double *coord){
	int n = sizeof(coord);
	int matched = 1;

	if (l == NULL){
		return 0;
	}
	double *coord1 = l->coordinates;
	
	double eps = .0001;
	int i = 0;
	for(i=0; i<n; i++){
		if(fabs(coord1[i] - coord[i]) > eps) {
			matched = 0;
		}
	}
	if (matched){
		return l->cost;
	} else {
		return memContains(l->next, coord);
	}
}

void freeMemNode(memNode m){
	assert(m!=NULL);
	free(m);
}
	
void freeAllNode(memNode m){
	while(m!=NULL){
		struct memNode *tmp = m;
		m = m->next;
		freeMemNode(tmp);
	}
}

void freeAllList(list l){
	freeAllNode(l->head);
	free(l);
}