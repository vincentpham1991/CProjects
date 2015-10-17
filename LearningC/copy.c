#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

void copy(char *inFile, char *outFile){
	FILE *inputFile = fopen(inFile,"rb");
	FILE *outputFile = fopen(outFile,"wb");
	/*
	if(inputFile = NULL || outputFile == NULL){
		fprintf(stderr, "could not open file: %s\n", strerror(errno));
		exit(1);
	}
	*/
	//char buff;
	
	while(!feof(inputFile)){
		char c = fgetc(inputFile);
		fputc(c, outputFile);
	}
	
	//printf("let's go");
	/*
	while((buff = fgetc(inputFile))!=EOF){
		//printf("we are here");
		//printf("%c",buff);
		fputc(buff, outputFile);
	}
	*/
	fclose(inputFile);
	fclose(outputFile);
}


void main(int argc, char** argv) {
    assert(argc==3);
    char* in_filename = argv[1];
    char* out_filename = argv[2];

	copy(in_filename, out_filename);



}
