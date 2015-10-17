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
	NPREFIX = 2,
	MULTIPLIER = 31,
	NHASH = 4097
};

typedef struct node *node;
struct node{
	char *str;
	int count;
	struct node *next;
};

/*hash: compute hash value of string*/
typedef struct hashTable *hashTable;
struct hashTable{
	int size;
	struct node **table;
};
struct node *mkNode(char *str, struct node *next){
	struct node *rv = (struct node *)malloc(sizeof(*rv));
	char *dupStr = strdup(str);
	rv->str = dupStr;
	rv->count = 1;
	rv->next = next;
	return rv;
}

/*
unsigned int hash(char *str){
	//char *dupStr = strdup(str);
	unsigned int h;
	unsigned char *p;
	
	h=0;
	for (p = (unsigned char *)str; *p != '0';p++){
		h += MULTIPLIER * h + *p;
	}
	return h % NHASH;
}
*/

unsigned int hash(char *str){
	
	unsigned int hashval = 0;
	for(;*str!='\0';str++){
		hashval+=*str;
	}
	return hashval % NHASH;
}


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

void add(struct hashTable *hashtable, char *str){
	char *dupStr = strdup(str);
	struct node *newNode;
	struct node *currNode;
	unsigned int hashval = hash(dupStr);
	//printf("new string: %s \t %d\n", dupStr, hashval);
	if((newNode = malloc(sizeof(struct node))) == NULL){
		return;
	}
	newNode->str = dupStr;
	newNode->count = 1;
	newNode->next = NULL;
	
	currNode = hashtable->table[hashval];
	/*
	if (currNode == NULL) {
		hashtable->table[hashval] = newNode;
		//mkNode(str,hashtable->table[hashval]);
		return;
	} else {*/
		struct node *tempNode = hashtable->table[hashval];
		while(tempNode != NULL) {
			
			if(strcmp(tempNode->str,dupStr)==0){
				(tempNode->count) = ((tempNode->count)+1);
				//printf("\nThis is me: %s, %s, %d\n", str, tempNode->str, tempNode->count);
				return;
			}
			tempNode = tempNode->next;
		}
		hashtable -> table[hashval] = mkNode(dupStr, hashtable ->table[hashval]);
		return;
	
	//return;
}

int findCount (struct hashTable *hashtable, char *str){
	struct node *newNode;
	char *dupStr = strdup(str);
	unsigned int hashval = hash(dupStr);
	//printf("\n\tin find count: %s: %d\n", dupStr, hashval);

	for(newNode = hashtable->table[hashval]; newNode != NULL; newNode = newNode->next){
		if(strcmp(newNode->str,dupStr)==0)
			return newNode->count;
	}
	return 0;	
}



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

struct hashTable *fillTable(char *text, int klen, int *alphabet){
	int tLen = strlen(text);
	char *circText = makeCircular(text,klen);
	char *kStr = malloc((klen+1)*sizeof(char));
	char *kplusStr = malloc((klen+2)*sizeof(char));
	//printf("Circ text: %s", circText);
	struct hashTable *hashtable = createTable();
	
	//int alphabet[256] = {};
	
	int i,j;
	for(i = 0; i < tLen; i++){
		for(j = 0; j < klen; j++)
			*(kStr + j) = circText[i+j];
		*(kStr + j) = '\0';
		add(hashtable,kStr);
	}
	
	for(i = 0; i < tLen; i++){
		alphabet[text[i]] = 1;
		for(j = 0; j < (klen+1); j++)
			*(kplusStr + j) = circText[i+j];
		*(kplusStr + j) = '\0';
		add(hashtable,kplusStr);
	}
	
	//printf("alphabet: %d\n", alphabet['c']);
	return hashtable;
}

double calcProb(struct hashTable *hashtable, char *mystery, int klen, int S) {
	int tLen = strlen(mystery);
	char *circText = makeCircular(mystery,klen);
	char *kStr = malloc((klen+1)*sizeof(char));
	char *kplusStr = malloc((klen+2)*sizeof(char));
	int nPC, nP;
	double prob = 0;
	double probTest;
	
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
		/*
		unsigned int hashval = hash(kStr);
		char *dupStr = strdup(kplusStr);
		unsigned int hashval2 = hash(dupStr);
		*/
		int nP = findCount(hashtable,kStr);
		int nPC = findCount(hashtable,kplusStr);
		
		/*
		printf("%d \t %d\n", nP, nPC);
		printf("%s \t %s\n", kStr, kplusStr);
		printf("%d \t %d\n", hashval, hashval2);
		*/
		
		prob += log((nPC + 1.0)/(nP + S));
		probTest = log((nPC + 1.0)/(nP + S));
		//printf("this is a prob %lf: \n", probTest);
	}
	return prob/tLen;
}

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

void printProb(FILE *speaker1, FILE *speaker2, FILE *mystery, int K) {
	char *s1 = getText(speaker1);
	char *s2 = getText(speaker2);
	char *unknown = getText(mystery);
	
	int alphabetS1[256] = {};
	int alphabetS2[256] = {};
	struct hashTable *hashTableS1 = fillTable(s1,K,alphabetS1);
	struct hashTable *hashTableS2 = fillTable(s2,K,alphabetS2);
	int alpha1 = countAlphabet(alphabetS1);
	int alpha2 = countAlphabet(alphabetS2);
	double probLik1 = calcProb(hashTableS1, unknown, K, alpha1);
	double probLik2 = calcProb(hashTableS2, unknown, K, alpha2);
	double diff = probLik1 - probLik2;
	printf("and the probability is...%lf\n", probLik1);
	printf("and for the second person... %lf\n", probLik2);
	printf("and the difference: %lf\n", diff);
	printf("%s %lf %lf %lf", mystery, probLik1, probLik2);
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
	
	double probLik1 = calcProb(hashTableS1, mystery, K, S1);
	double probLik2 = calcProb(hashTableS2, mystery, K, S2);
	double diff = probLik1 - probLik2;
	
	printf("%s %lf %lf %lf", argv[3], probLik1, probLik2, diff);
	//printProb(f1, f2, f3, K);
	
	return 0;
}