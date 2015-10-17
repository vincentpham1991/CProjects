/* utility routine that checks the return value from
 * malloc and generates an error if malloc returns NULL 
 */
void *ckMalloc(int size);

/* utility routine that checks the return value from
 * realloc and generates an error if realloc returns NULL 
 */
void *ckRealloc(void *data, int size);

/* getText: return a string containing the contents of the 
 * specified file.   Fails and exits, if it runs out of space
 * to hold the file.
 */
char *getText(FILE *f);
