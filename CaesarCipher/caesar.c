#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

bool isUpper(char c) {
    return c>='A' && c<='Z';
}

bool isLower(char c) {
    return c>='a' && c<='z';
}

char shiftChar(char c, int shift) {
    char base;
    if(isUpper(c))
        base = 'A';
    else if(isLower(c))
        base = 'a';
    else
        return c;
    c -= base;
    
    c = c+shift;
    while(c<0)
        c+=26;
    while(c>=26)
        c-=26;

    return base + c;
}

void copy(char *inFile, char *outFile, int shift){
	FILE *inputFile = fopen(inFile,"rb");
	FILE *outputFile = fopen(outFile,"wb");

	
	while(!feof(inputFile)){
		char c = fgetc(inputFile);
		char shiftc = shiftChar(c, shift);
		fputc(shiftc, outputFile);
	}

	fclose(inputFile);
	fclose(outputFile);
}

void main(int argc, char** argv) {
    assert(argc==4);
    char* in_filename = argv[1];
    char* out_filename = argv[2];
    int shift = atoi(argv[3]);
    

	copy(in_filename, out_filename, shift);



}
