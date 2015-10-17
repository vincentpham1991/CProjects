#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
	char character = 1;
	int number = (int)character;
	while (number < 128){
		printf("%c, %d \n", character, number);
		character++;
		number++;
	}
	return 0;
}