//Name: Vincent Pham, Sili Wen

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "util.h"

enum {
	MULTIPLIER = 31,
	NHASH = 40971
};

typedef struct node *node;
struct node{
	char *str;
	int count;
	struct node *next;
};

typedef struct hashTable *hashTable;
struct hashTable{
	int size;
	struct node **table;
};

/*mkNode: makes a new node*/
struct node *mkNode(char *str, struct node *next){
	struct node *rv = (struct node *)malloc(sizeof(*rv));
	rv->str = str;
	rv->count = 1;
	rv->next = next;
	return rv;
}

/*hash: compute hash value of string*/
unsigned int hash(char *str){
	unsigned int h;
	unsigned char *p;
	
	h=0;
	for (p = (unsigned char *)str; *p != '\0';p++){
		h = MULTIPLIER * h + *p;
	}
	return h % NHASH;
}

/*createTable: make a new hash Table with size NHASH all initialized to NULL*/
struct hashTable *createTable() {
	struct hashTable *newTable;
	
	if((newTable=malloc(sizeof(struct hashTable))) == NULL){
		return NULL;
	}
	
	if((newTable->table = malloc(sizeof(struct node *) * NHASH)) == NULL) {
		return NULL;
	}
	
	int i;
	for(i=0; i<NHASH; i++){
		newTable->table[i] = '\0';
	}
	newTable->size = NHASH;
	return newTable;
}

/*add: increment the count of a string in a hash table
*		or adds a new string to the table
*/
void add(struct hashTable *hashtable, char *str){
	char *dupStr = strdup(str);
	unsigned int hashval = hash(dupStr);

	struct node *tempNode = hashtable->table[hashval];
	while(tempNode != NULL) {
		if(strcmp(tempNode->str,dupStr)==0){
			(tempNode->count) = ((tempNode->count)+1);
			return;
		}
		tempNode = tempNode->next;
	}
	hashtable -> table[hashval] = mkNode(dupStr, hashtable ->table[hashval]);
	return;
}

/*findCount: find the number of time a string has occurred*/
int findCount (struct hashTable *hashtable, char *str){
	struct node *newNode;
	unsigned int hashval = hash(str);

	for(newNode = hashtable->table[hashval]; newNode != NULL; newNode = newNode->next){
		if(strcmp(newNode->str,str)==0)
			return newNode->count;
	}
	return 0;	
}

/*makeCircular: connects the end of a string to the beginning*/
char *makeCircular(char *text, int klen){
	int size = strlen(text);
	int newSize = size + klen;
	char *circText = malloc((newSize+1)*sizeof(char));
	
	int i,j;
	for(i=0; i < size; i++)
		*(circText + i) = *(text + i);
		
	for(j = 0; j < klen; j++, i++)
		*(circText + i) = *(text + j);
	
	*(circText + i) = '\0';
	return circText;
}

/*fillTable: fills the hash table with k and k+1 strings in the corpus,
*				also checks what alphabets are in the corpus
*/
struct hashTable *fillTable(char *text, int klen, int *alphabet){
	int tLen = strlen(text);
	char *circText = makeCircular(text,klen);
	char *kStr = malloc((klen+1)*sizeof(char));
	char *kplusStr = malloc((klen+2)*sizeof(char));
	struct hashTable *hashtable = createTable();
	
	int i,j;
	for(i = 0; i < tLen; i++){
		alphabet[text[i]] = 1;
		for(j = 0; j < (klen+1); j++){
			if(j != klen){
				*(kStr + j) = circText[i+j];
			} else {
				*(kStr + j) = '\0';
			}
			*(kplusStr + j) = circText[i+j];
		}
		*(kplusStr + j) = '\0';
		add(hashtable, kStr);
		add(hashtable,kplusStr);
	}
	return hashtable;
}

/*counts the number alphabets there are in the corpus*/
int countAlphabet(int alphabet[256]){
	int count = 0;
	int i;
	for(i=0; i<256; i++){
		if(alphabet[i]){
			count++;
		}
	}	
	return count;
}

/*calcProbAvg: calculates the average log likelihood*/
double calcProbAvg(struct hashTable *hashtable, char *mystery, int klen, int S) {
	int tLen = strlen(mystery);
	char *circText = makeCircular(mystery,klen);
	char *kStr = malloc((klen+1)*sizeof(char));
	char *kplusStr = malloc((klen+2)*sizeof(char));
	int nPC, nP;
	double prob = 0;
	
	int i,j;
	for(i = 0; i < tLen; i++){
		for(j = 0; j < (klen+1); j++) {
			if(j !=klen) {
				*(kStr + j) = circText[i+j];
			} else {
				*(kStr + j) = '\0';
			}
			*(kplusStr + j) = circText[i+j];
		}
		*(kplusStr + j) = '\0';
		nP = findCount(hashtable,kStr);
		nPC = findCount(hashtable,kplusStr);

		prob += log((nPC + 1.0)/(nP + S));
	}
	return prob/tLen;
}

int main(int argc, char *argv[]) {
	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2],"r");
	FILE *f3 = fopen(argv[3],"r");
	int K = atoi(argv[4]);
	if (f1 == NULL) {
		fprintf(stderr,"Failed to open: %s\n", argv[1]);
		exit(2);
	}
	if (f2 == NULL) {
		fprintf(stderr,"Failed to open: %s\n", argv[2]);
		exit(2);
	}
	if (f3 == NULL) {
		fprintf(stderr,"Failed to open: %s\n", argv[3]);
		exit(2);
	}

	char *speaker1 = getText(f1);
	char *speaker2 = getText(f2);
	char *mystery = getText(f3);
	
	int sTracker1[256] = {};
	int sTracker2[256] = {};
	
	struct hashTable *hashTableS1 = fillTable(speaker1,K,sTracker1);
	struct hashTable *hashTableS2 = fillTable(speaker2,K,sTracker2);
	
	int S1 = countAlphabet(sTracker1);
	int S2 = countAlphabet(sTracker2);
	
	double probLik1 = calcProbAvg(hashTableS1, mystery, K, S1);
	double probLik2 = calcProbAvg(hashTableS2, mystery, K, S2);
	double diff = probLik1 - probLik2;
	
	printf("%s %lf %lf %lf", argv[3], probLik1, probLik2, diff);
	
	return 0;
}