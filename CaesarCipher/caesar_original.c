#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool isUpper(char c){
	if((c >= 'A') && (c <= 'Z')){
		return true;
	} else {
		return false;
	}
}


bool isLower(char c){
	if((c >= 'a') && (c <= 'z')){
		return true;
	} else {
		return false;
	}
}

char shiftChar(char c, int n){
	int cLower = isLower(c);
	int cUpper = isUpper(c);
	while(n >= 26) {
		n -= 26;
	}
	
	while(n <0) {
		n += 26;
	}
	
	if (cLower == 1) {
		if ((c + n) > 'z') {
			char diff = 'z' - c;
			c = 'a' + n - 1 - diff;
			return c;
		} else {
			return c + n;
		}
	}
	else if (cUpper == 1) {
		if ((c+n) > 'Z') {
			char diff = 'Z' - c;
			c = 'A' + n - 1 - diff;
			return c;
		} else {
			return c + n;
		}
	}
	else {
		return c;
	}
			
}

void encryptInPlace(char *message, int n){
	int size = strlen(message);
	int i;
	for (i=0; i < size; i++) {
		*(message+i) = shiftChar(*(message+i),n);
	}
	
}

char *encrpytNewMemory(char *message, int n){
	int size = strlen(message);
	char *newM = malloc((size+1)*sizeof(char));
	
	int i;
	for (i=0;i<size;i++){
		*(newM + i) = shiftChar(*(message+i),n);
	}
	*(newM + i) = '\0';
	return newM;
} 
void test()
{
    char text[] = "Some simple text"; // create an array of characters
    char *pointer  = &text[0]; // create a pointer, named pointer, set it's value to the location of the first element using the & operator.
    int i = 0;
    for(i=0; i < 40; i++) { // note that 40 is greater than the length of our string
        pointer++; // move the pointer up one character. Put BreakPoint here.
    }
}
int main (int argc, char *argv[]){
	//int c = sizeof("one");
	int a = isUpper('a');
	int b = isLower('a');
	int c = isUpper('Z');
	int d = isLower('Z');
	char r = shiftChar('.', 1);
	printf("%d %d %d %d\n", a,b,c,d);
	printf("%c\n", r);
	
	char *message = strdup("M! K fobi pkcd, iod kd dswoc pbecdbkdsxq vkxqekqo.");
	printf("message    : %s\n", message);
	encryptInPlace(message,-10);
	printf("ciphertext : %s\n\n", message);
	
	char *message1 = strdup("M! K fobi pkcd, iod kd dswoc pbecdbkdsxq vkxqekqo.");
	printf("message    : %s\n", message1);
	char *ciphertext = encrpytNewMemory(message1,-10);
	printf("ciphertext : %s\n", ciphertext);
	free(ciphertext);
	printf("ciphertext : %s\n", ciphertext);
	test();
	return 0;
}