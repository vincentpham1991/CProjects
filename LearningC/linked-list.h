#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct list *list_t;

/* mkList:
 *  create a new list
 */
list_t mkList();

/* freeAllList:
 *  free the list 
 */
void freeAllList(list_t l);

/* isEmptyList: 
 *   returns true if the list is empty, false otherwise
 */
bool isEmptyList(list_t l);

/* printList:
 *   prints the contents of the list on one line.
 */
void printList(list_t l);

/* addToList:
*	Add val to the list
*/
void addToList(list_t l, char *val);

/* addToFrontList:
 *   Add val to the front of list l
 */
void addToFrontList(list_t l, int val);

/* addToRearList:
 *   Add val to the end of list l
 */
void addToRearList(list_t l, int val);

/* removeFromList:
 *   remove the first occurrence of val from the list, if it
 *   occurs in the list.
 */
void removeFromList(list_t l, char *val);

#endif
