
#include <stdio.h>
#include <string.h>


#include <stdlib.h>
//#include <stdbool.h>
//#include <assert.h>


#include "linked-list.h"

int main() {
    list_t list = mkList();
    char *dog = strdup("dog");
    char *cat = strdup("cat");

    addToList(list, "cat");
    printList(list); 

    addToList(list, "anteater");
    printList(list); 

    addToList(list, dog);
    dog[0] = 'f'; // should not change the string in the list
    printList(list); 

    addToList(list, "aardvark");
    printList(list); 

    removeFromList(list, "dog");
    printList(list);

    // uncomment to test stringInList
     //printf("Is dog in the list? %s\n", stringInList(list, "dog")?"YES":"NO");
     //printf("Is cat in the list? %s\n", stringInList(list, cat)?"YES":"NO");

    free(cat);
    free(dog); // good dogs don't leak
    freeAllList(list);
    return 0;
}
