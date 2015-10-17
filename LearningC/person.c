#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct person_s *Person;

struct person_s {
    char* firstName;
    char* lastName;
    int age; 
    double weight;
};

Person mkPerson(char* firstName, char* lastName, int age, double weight) {
    Person p = malloc(sizeof(struct person_s));
    p->firstName = firstName;
    p->lastName = lastName;
    p->age = age;
    p->weight = weight;
	return p;
}

Person* createSimpsons() {
    // Make an array of pointers to people
    Person* people = malloc(sizeof(Person)*5);
    people[0] = mkPerson("Maggie", "Simpson", 1, 20);
    people[1] = mkPerson("Bart", "Simpson", 9, 90.3);
    people[2] = mkPerson("Lisa", "Simpson", 7, 53.4);
    people[3] = mkPerson("Homer", "Simpson", 31, 203.1);
    people[4] = mkPerson("Marge", "Simpson", 31, 140.2);

    return people;
}
void printPerson(Person person){
	fprintf(stdout,"%s, %s, %d, %lf\n", person->firstName, person->lastName, person->age, person->weight);
	return;
}

void printPersonToFile(FILE* file, Person person){
	
	fprintf(file,"%s, %s, %d, %lf\n", person->firstName, person->lastName, person->age, person->weight);
}

Person* loadPerson(FILE* file){
	Person* people = malloc(sizeof(Person)*5);
	char *lastName = malloc(20*sizeof(Person));
	char *firstName = malloc(20*sizeof(Person));
	int age;
	double weight;
	
	int i;
	for(i = 0; i < 5; i++){
		fscanf(file, "%19[a-zA-Z], %19[a-zA-Z], %d, %lf\n", lastName, firstName, &age, &weight);
		people[i] = mkPerson(lastName,firstName,age,weight);
		printPerson(people[i]);
	}
	
	return people;
}
int main(int argc, char *argv[]) {
    // Add code here.
	FILE* outputFile = fopen("output.txt","w");
	if(outputFile == NULL){
		fprintf(stderr,"Could not open output.txt: %s\n", strerror(errno));
		exit(1);
	}
	
	FILE* inputFile = fopen("bouvier.txt","r");
	if(inputFile == NULL){
		fprintf(stderr,"Could not open file: %s\n", strerror(errno));
		exit(1);
	}
	
	Person *bouviers = loadPerson(inputFile);
	
	
	Person *people = createSimpsons();
	
	printPersonToFile(outputFile, people[1]);
	printPersonToFile(stdout, people[1]);
	fclose(outputFile);
	int i;
	//printPerson(people);
	
	for(i=0; i<5; i++){
		printPerson(people[i]);
	}
	
    

    // Don't forget to free the memory created by createSimpsons! 
    return 0;
}
