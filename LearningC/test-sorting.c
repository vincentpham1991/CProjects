
#include <stdio.h>

#include "linked-list.h"

int main() {
    list_t list = mkList();

    addToList(list, 5);
    printList(list); 

    addToList(list, 3);
    printList(list); 

    addToList(list, 4);
    printList(list); 

    addToList(list, 6);
    printList(list); 

    addToList(list, 1);
    printList(list); 

    addToList(list, 2); 
    printList(list); 

    freeAllList(list);

    return 0;
}
