#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list *list;

//makes a list
list *mkList();

//puts the new coordinate and cost into the list at the front of the list
void addToList(list l, int cost, double *coordinates);

//part of the print test
void printList(list *l);

//check if coordinates is in list
int listContains(list l, double *coord);

//Frees the entire list
void freeAllList(list l);

#endif