#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

char *mystrcat(char *A, char *B){
	int sizeA = strlen(A);
	int sizeB = strlen(B);
	int totalSize = sizeA + sizeB + 1;
	char *something = malloc(totalSize*sizeof(char));
	
	int i;
	for(i=0; i < sizeA; i++){
		*(something + i) = *(A + i);
	}
	int j = 0;
	for(;i < sizeA + sizeB; i++){
		*(something + i) = *(B + j);
		j++;
	}
	*(something + i) = '\0';
	
	return something;
	
	//return (int *)totalSize;
	//char c = 'A';
	//return 'A';
}


int main (int argc, char *argv[]){
	//int c = sizeof("one");
	char *c = mystrcat("boo", "go");
	printf("%s", c);
	return 0;
}