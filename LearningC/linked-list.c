/* CS122 W'13 
     Lecture #6

     Structures, Pointers to Structures, and Linked Lists.
     Type def
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "linked-list.h"

struct node {
    int val;
    struct node *next;
};

typedef struct node *node_t;

// PROTOTYPES

static bool isEmptyNode(node_t n);
static void printNode(node_t n);
//static struct node *addToFrontNode(node_t n, int val);
//static struct node *addToRearNode(node_t n, int val);
static struct node *addToNode(node_t n, int val);
static struct node *removeFromNode(node_t n, int val);

static void freeNode(node_t n);
static void freeAllNode(node_t n);


struct list {
    struct node *head;
};

list_t mkList() {
    list_t rv = (list_t) malloc(sizeof(*rv));    
    rv->head = NULL;
    return rv;
}

void freeAllList(list_t l) {
    freeAllNode(l->head);
    free(l);
}


bool isEmptyList(list_t l) {
    return isEmptyNode(l->head);
}

void printList(list_t l) {
    printNode(l->head);
}

void addToList(list_t l, int val) {
	l ->head = addToNode(l->head,val);
}
/*
void addToFrontList(list_t l, int val) {
    l->head = addToFrontNode(l->head, val);
}

void addToRearList(list_t l, int val) {
    l->head = addToRearNode(l->head, val);
}
*/
void removeFromList(list_t l, int val) {
    l->head = removeFromNode(l->head, val);
}


struct node *mkNode(int val, node_t next) {
    struct node *rv = (struct node *) malloc(sizeof(*rv));
    rv->val = val;
    rv->next = next;
    return rv;
}


static bool isEmptyNode(struct node *l) {
    return (l == NULL);
}

static int lenNode(struct node *l) {
    int len = 0;
    while (l != NULL) {
        len++;
        l = l->next;
    }
    return len;
}


static void printNode(struct node *l) {
    while (l != NULL) {
        printf("%d ", l->val);
        l = l->next;
    }
    printf("\n");
}
/*
static struct node *addToFrontNode(struct node *l, int val) {
    return mkNode(val, l);
}

static struct node *addToRearNode(struct node *l, int val) {
    if (isEmptyNode(l)) {
        return addToFrontNode(l, val);
    } else {
        struct node *head = l;
        while (l->next != NULL) {
            l = l->next;
        }
        l->next = mkNode(val, NULL);
        return head;
    }
}
*/
static struct node *addToNode(struct node *l, int val) {
	if (isEmptyNode(l)) {
		return mkNode(val, l);
	} 
	if (val < l->val){
		return mkNode(val,l);
	}
	else {
		int check = 0;
		struct node *head = l;
		struct node *curr = l -> next;
		
		while (l ->next != NULL  && check == 0) {
			//curr = curr -> next;
			//l = l ->next;
			if(val < curr -> val) {
				l -> next = mkNode(val,l -> next);
				//l = curr ->next;
				//return head;
				return head;
				check = 1;
			} 
			curr = curr -> next;
			l = l -> next;

			//l = l ->next;
		}
		if (check == 0){
			l->next = mkNode(val,NULL);
		}
		return head;
	}
	
}

static struct node *removeFromNode(struct node *l, int val) {
    if (isEmptyNode(l))
        // not in node
        return NULL;
    
    if (l->val == val) {
        // val at head of the node
        struct node *head = l->next;
        freeNode(l);
        return head;
    }

    struct node *head = l;
    struct node *prev = l;
    struct node *curr = l->next;

    while (curr != NULL) {
        if (curr->val == val) {
            prev->next = curr->next;
            freeNode(curr);
            return head;
        }
        
        prev = curr;
        curr = curr->next;
    }

    // not in node
    return head;
}

void freeNode(node_t n) {
    assert(n!=NULL);
    free(n);
}

void freeAllNode(node_t n) {
    while (n != NULL) {
        struct node *tmp = n;
        n = n->next;
        freeNode(tmp);
    }
}

