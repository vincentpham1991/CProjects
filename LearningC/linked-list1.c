/* CS122 W'13 
     Lecture #6

     Structures, Pointers to Structures, and Linked Lists.
     Type def
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "linked-list.h"

struct node {
    char *val;
    struct node *next;
};

typedef struct node *node_t;

// PROTOTYPES

static bool isEmptyNode(node_t n);
static void printNode(node_t n);
//static struct node *addToFrontNode(node_t n, char *val);
//static struct node *addToRearNode(node_t n, char *val);
static struct node *addToNode(node_t n, char *val);
static struct node *removeFromNode(node_t n, char *val);

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

void addToList(list_t l, char *val) {
	char *value;
	value = strdup(val);
	l ->head = addToNode(l->head,value);
	//free(value);
}
/*
void addToFrontList(list_t l, char *val) {
    l->head = addToFrontNode(l->head, val);
}

void addToRearList(list_t l, char *val) {
    l->head = addToRearNode(l->head, val);
}
*/
void removeFromList(list_t l, char *val) {
    l->head = removeFromNode(l->head, val);
}


struct node *mkNode(char *val, node_t next) {
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
        printf("%s ", l->val);
        l = l->next;
    }
    printf("\n");
}
/*
static struct node *addToFrontNode(struct node *l, char *val) {
    return mkNode(val, l);
}

static struct node *addToRearNode(struct node *l, char *val) {
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
static struct node *addToNode(struct node *l, char *val) {
	//char *string = strdup(val);
	if (isEmptyNode(l)) {
		return mkNode(val, l);
	} 
	if (strcmp(val,l->val) < 0){
		return mkNode(val,l);
	}
	else {
		int check = 0;
		struct node *head = l;
		struct node *curr = l -> next;
		
		while (l ->next != NULL  && check == 0) {
			//curr = curr -> next;
			//l = l ->next;
			if(strcmp(val,curr -> val) <0) {
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

static struct node *removeFromNode(struct node *l, char *val) {
    if (isEmptyNode(l))
        // not in node
        return NULL;
    
    if (strcmp(l->val,val) == 0) {
        // val at head of the node
        struct node *head = l->next;
        freeNode(l);
        return head;
    }

    struct node *head = l;
    struct node *prev = l;
    struct node *curr = l->next;

    while (curr != NULL) {
        if (strcmp(curr->val,val) == 0) {
            prev->next = curr->next;
			//free(string);
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

