//Author: Vincent Pham
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

//#include "bst.h"
//#include "linked-list.h"

/* utility routine that checks the return value from
 * malloc and generates an error if malloc returns NULL 
 */
void *ckMalloc(int size) {
  void *rv = malloc(size);
  if (rv == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(2);
  }
  return rv;
}

typedef struct node *treeNode;
/*creates the node structure for the binomial search tree
*	with three values direction to split, left node, and right node
*/
struct node{
	int dir;
	int type;
	treeNode left;
	treeNode right;
};

/* adds a new treeNode, dir is set to -1 since a directional split
* is not found yet
*/
struct node *addTreeNode(){
	struct node *rv = (struct node *)malloc(sizeof(*rv));
	rv->dir = -1;
	rv->type = -1; //type is  -1 till the end of the tree
	rv->right = 0;
	rv->left = 0;
	return rv;
}
	
// makes a tree Node
treeNode mkNode(int dir, int type, treeNode left, treeNode right){
	struct node *rv = (struct node *)malloc(sizeof(*rv));
	rv->dir = dir;
	rv->type = type;
	rv->left = left;
	rv->right = right;
}

void freeTree(struct node *n){
	if(n == NULL)
		return;
	freeTree(n->right);
	freeTree(n->left);
	
	free(n);
}

typedef struct list *list;
/*structure for the memoization node that keeps in the next node, 
* cost of splitting, and coordinates in the format lb_0, lb_1,...ln_N-1,
* ub_0, ub_1,...ub_N-1
*/
typedef struct memNode *memNode;
struct memNode{
	struct memNode *next;
	//int type;
	int cost;
	double *coordinates;
};

//structure list to keep the memoization nodes linked
struct list{
	struct memNode *head;
};

//makes a list
struct list *mkList(){
	struct list *rv = (struct list *)malloc(sizeof(*rv));
	rv->head = NULL;
	return rv;
};

//adds a mew coordinate and cost to the memoization
struct memNode *addToMem(struct memNode *next, int cost, double *coordinates){
	struct memNode *rv = (struct memNode *)malloc(sizeof(*rv));
	int n = sizeof(coordinates);
	rv->coordinates=malloc(sizeof(double)* n);
	rv->next = next; 
	rv->cost = cost;
	
	int i;
	for (i = 0; i < n; i++){
		rv->coordinates[i] = coordinates[i];
	}
	return rv;
}

//puts the new coordinate and cost into the list at the front of the list
void addToList(struct list *l, int cost, double *coordinates){
	l->head = addToMem(l->head,cost, coordinates);
}

//print test for memoization
void printMemNode(struct memNode *l){
	while (l != NULL) {
		printf("%d ", l->cost);
		int n = sizeof(l->coordinates);
		int i;
		for(i = 0; i < n; i++){
			printf("%lf ", l->coordinates[i]);
		}
		l = l->next;
		printf("\n");
	}
	printf("\n");
}

//part of the print test
void printList(struct list *l){
	printMemNode(l->head);
}

//check if coordinates is in list
int listContains(list l, double *coord){
	return memContains(l->head, coord);
}

//check if coordinate is in list
int memContains(memNode l, double *coord){
	int n = sizeof(coord);
	int matched = 1;

	if (l == NULL){
		return 0;
	}
	double *coord1 = l->coordinates;
	
	double eps = .0001;
	int i = 0;
	for(i=0; i<n; i++){
		if(fabs(coord1[i] - coord[i]) > eps) {
			matched = 0;
		}
	}
	if (matched){
		return l->cost;
	} else {
		return memContains(l->next, coord);
	}
}

void freeMemNode(memNode m){
	assert(m!=NULL);
	free(m);
}
	
void freeAllNode(memNode m){
	while(m!=NULL){
		struct memNode *tmp = m;
		m = m->next;
		freeMemNode(tmp);
	}
}

void freeAllList(list l){
	freeAllNode(l->head);
	free(l);
}

//get # columns
int getCols(char *fn){
  	FILE *f = fopen(fn, "r");
  	if (f == NULL)
    	return 0;
  	int cnt = 0;
  	double d;
  	char c[2];
  	while (fscanf(f, "%lf", &d) == 1) {
    	int v = fscanf(f, "%[,\n]", c);
    	cnt++;
    	if ((v != 1) || (c[0] == '\n'))
      	break;
    	}
  	fclose(f);
  	return cnt;
}

//get # of rows
int getRows(char *fn){
	FILE *f = fopen(fn, "r");
	if (f == NULL)
    	return 0;

	int cnt = 0;
	while(!feof(f)){
		char c = fgetc(f);
		if(c == '\n')
			cnt++;
	}
	fclose(f);
	return cnt;
}

/* inRect: given a point and a hypercube, return if the point is contained within the hypercube*/
int inRect(double *pointCoords, double *rectCoords, int dim){
	int i=0;
	
	while(i<dim){
		if ((pointCoords[i] <= rectCoords[i]) || (pointCoords[i] > rectCoords[i+dim]))
			return 0;
		i++;
	}
	return 1;
}

//calculates the midpoint of a line
double cutHalf(double *coord, int axis, int dim){
	double halfPoint = (coord[axis+dim]-coord[axis])/2+coord[axis];
	return halfPoint;
}

/* minCost: computes the cost of the path with the least cost*/
int minCost(list l, double *coord, double **data, int rows, int dim, int level, int lambda){
	int numElem = dim*2;
	int cost = findCost(l, coord, data, rows, dim, lambda);
	int newCost = cost;
	int tempCost; //place holder for deciding which axis cost to keep
	double halfPoint;
	
	double *coordLeft = ckMalloc(sizeof(double)*numElem);
	double *coordRight = ckMalloc(sizeof(double)*numElem);
	
	//if cost < 2*lambda, then it is already more efficient then splitting even further
	if(level == 1 || cost <= 2*lambda){
		return cost;
	} else {
		level--;
	}
	
	int i,j;
	for(i = 0; i <dim; i++){
		for(j = 0; j<numElem; j++){
			coordLeft[j] = coord[j];
			coordRight[j] = coord[j];
		}		
		halfPoint = cutHalf(coord, i, dim);			
		coordLeft[i+dim] = halfPoint;
		coordRight[i] = halfPoint;
		tempCost = minCost(l, coordLeft,data, rows, dim, level, lambda) + minCost(l, coordRight, data, rows, dim, level, lambda);
		if(newCost > tempCost){
			newCost = tempCost;
		}			
	}
	free(coordLeft);
	free(coordRight);
	return newCost;
	
}

/*check if coordinate is in List,
*	if it is, returns its 
*	if not, add its coordinate and cost into the list 
*/	
int findCost(list l, double *coord, double **data, int rows, int dim, int lambda){
	int cost;
	int costExist = listContains(l,coord);
	if(costExist == 0){
		cost = getError(coord, data, rows, dim) + lambda;
		addToList(l,cost, coord);
	} else {
		cost = costExist;
	}	
	return cost;
}

//gets the error of the rectangle
int getError(double *coord, double **data, int rows, int dim){
	int cat1, cat2;//category for datapoints
	cat1 = cat2 = 0;
	int i, j;
	double *dataPt = ckMalloc(sizeof(double)*dim);
	for(i=0; i<rows; i++){
		for(j=0; j<dim; j++){
			dataPt[j] = data[i][j];
		}
		if (inRect(dataPt, coord, dim)){
			if (data[i][dim] == 0)
				cat1++;
			else 
				cat2++;
		}
	}
	free(dataPt);
	if(cat1>cat2)
		return cat2;
	else
		return cat1;
}

//gets the type of the rectangle
int getType(double *coord, double **data, int rows, int dim){
	int cat1, cat2;//category for datapoints
	cat1 = cat2 = 0;
	int i, j;
	double *dataPt = ckMalloc(sizeof(double)*dim);
	for(i=0; i<rows; i++){
		for(j=0; j<dim; j++){
			dataPt[j] = data[i][j];
		}
		if (inRect(dataPt, coord, dim)){
			if (data[i][dim] == 0)
				cat1++;
			else 
				cat2++;
		}
	}
	free(dataPt);
	if(cat1>cat2)
		return 0;
	else
		return 1;
}

/*obtain the recursive cost using data from memoization
* and returns a lower cost if available
*/
int sumCost(list l, double *coord, int dim, int prevCost, int level){
	int numElem = dim*2;
	int tempCost = prevCost;
	int costLeft, costRight;
	int costTempL, costTempR, totalCost;
	double midPoint;

	
	if(level == 1 || prevCost == 0){
		return 0;
	}
	level = level -1;
	
	double *coordLeft = ckMalloc(sizeof(double)*numElem);
	double *coordRight = ckMalloc(sizeof(double )*numElem);
	double *coordTempL = ckMalloc(sizeof(double)*numElem);
	double *coordTempR = ckMalloc(sizeof(double)*numElem);
	
	int i,j;
	for(i=0; i < dim; i++){
		for(j = 0; j<numElem; j++){
			coordLeft[j] = coord[j];
			coordRight[j] = coord[j];
		}
		midPoint = cutHalf(coord,i,dim);
		coordLeft[i+dim] = midPoint;
		coordRight[i] = midPoint;
		
		costLeft = listContains(l,coordLeft);
		costRight = listContains(l,coordRight);
		totalCost = costLeft + costRight;
		
		if(totalCost < tempCost && (costLeft!=0 && costRight != 0)){
			tempCost = totalCost;
			for(j = 0; j<numElem; j++){
					coordTempL[j] = coordLeft[j];
					coordTempR[j] = coordRight[j];
			}  
		} else {
			costTempL = sumCost(l, coordLeft, dim, costLeft, level);
			costTempR = sumCost(l, coordRight, dim, costRight, level);
			
			if((costTempL + costTempR < tempCost) && ((costTempL!=0 && costTempR) != 0)){
				tempCost = totalCost;
			}
		}
	}
	free(coordLeft);
	free(coordRight);
	free(coordTempL);
	free(coordTempR);
	return tempCost;
}

//obtain the tree with the types at the end
treeNode growTree(treeNode root, list l, double **data , int rows, double *coord, int dim, int level){
	int numElem = dim*2;
	int cost = listContains(l,coord);
	int tempCost = cost;
	int costLeft,costRight;
	double midPoint;
	
	double *coordLeft = ckMalloc(sizeof(double)*numElem);
	double *coordRight = ckMalloc(sizeof(double )*numElem);
	double *coordTempL = ckMalloc(sizeof(double)*numElem);
	double *coordTempR = ckMalloc(sizeof(double)*numElem);
	
	int check = 0; 
	int possibleMoves = sumCost(l, coord, dim, cost, level);
	
	int costL, costR, totalCost;
	
	//if there is no better options, set as final node;
	if (possibleMoves == 0 || possibleMoves == cost){
		root->dir = -1;
		root->type =getType(coord, data, rows, dim);
		return root;
	}
	int i, j;
	for(i = 0; i <dim; i++){
		memcpy(coordLeft,coord,numElem*sizeof(double));
		memcpy(coordRight,coord,numElem*sizeof(double));
		
		midPoint = cutHalf(coord, i, dim);
		coordLeft[i+dim] = midPoint;
		coordRight[i] = midPoint;
		costLeft = listContains(l,coordLeft);
		costL = sumCost(l,coordLeft,dim,costLeft, level);

		costRight = listContains(l,coordRight);
		costR = sumCost(l,coordRight, dim, costRight, level);

		totalCost = costL + costR;

		if(i==0 || totalCost < tempCost){
			if((costL != 0) && (costR != 0)){
				tempCost = totalCost;
				root->dir = i;

				memcpy(coordTempL,coordLeft,numElem*sizeof(double));
				memcpy(coordTempR,coordRight,numElem*sizeof(double));
				check = 1;
			}
		}
	}
	if(check !=0) {
		root->right = addTreeNode();
		growTree(root->right, l,data, rows, coordTempR, dim, level-1);
		root->left = addTreeNode();
		growTree(root->left, l,data, rows, coordTempL, dim, level-1);
	}
	free(coordLeft);
	free(coordRight);
	free(coordTempL);
	free(coordTempR);
	
	return root;
}

//use training data for partitioning and returns the tree 
treeNode trainData(char *fn, int lambda, int level){
	int rows = getRows(fn);
	int cols = getCols(fn);
	int dim = cols - 1;
	
	double myvariable;
	double **dataPoints = ckMalloc(sizeof(double *)*rows);
	int i;
	for(i=0; i < rows; i++){
		dataPoints[i] = ckMalloc(cols*sizeof(double));
	}

	FILE *f = fopen(fn, "r");
  	if (f == NULL)
    	return;
		
	int j;
	
	//obtain the data, and place into an array
	for(i = 0; i < rows; i++) {
		for (j = 0 ; j < cols; j++) {
			fscanf(f,"%lf,",&myvariable);
			dataPoints[i][j] = myvariable;
	    }
	}

	
	double *coordinate = ckMalloc(sizeof(double)*dim*2);
	
	//set initial coordinates;
	for(i=0; i < dim; i++){
		coordinate[i] = 0.0;
	}
	for(i=dim; i < dim*2; i++){
		coordinate[i]=1.0;
	}
	
	list l = mkList();
	int minVal = minCost(l, coordinate, dataPoints, rows, dim, level, lambda);
	
	treeNode root = addTreeNode();
	treeNode grownRoot = growTree(root, l, dataPoints, rows, coordinate, dim, level);

	free(coordinate);
	free(dataPoints);
	freeAllList(l);
	
	fclose(f);
	return grownRoot;
}

//prints the final partiions into a file
void printPartitions(FILE *outputFile, treeNode tree, int dim, double *coord){
	int numElem = dim*2;
	int midPoint;
	
	double *coordL = ckMalloc(sizeof(double)*numElem);
	double *coordR = ckMalloc(sizeof(double)*numElem);
	
	int direction = tree->dir;
	
	memcpy(coordL,coord,numElem*sizeof(double));
	memcpy(coordR,coord,numElem*sizeof(double));

	if(direction == -1){
		int i;
		for(i = 0; i < numElem; i++){
			fprintf(outputFile,"%lf ", coord[i]);
		} 
		fprintf(outputFile,"%d\n", tree->type);
	} else{
		midPoint = cutHalf(coord,direction,dim);
		coordL[direction+dim] = midPoint;
		coordR[direction] = midPoint;
		printPartitions(outputFile, tree->left, dim, coordL);
		printPartitions(outputFile, tree->right, dim, coordR);
	}
	
	free(coordL);
	free(coordR);
}

//returns the accuracy of the partition on a testing file
double reportAccuracy(char *testingFile, treeNode tree, int dim, double *coord){
	int numElem = dim*2;
	int dir;
	int count = 0;
	
	int rows = getRows(testingFile);
	int cols = getCols(testingFile);
	
	double midPoint;
	double myvariable;
	int myType;
	double *dataPoints;
	dataPoints = ckMalloc(sizeof(double)*cols);

	FILE *f = fopen(testingFile, "r");
  	if (f == NULL)
    	return;
		
	treeNode treecpy;

	double *nextCoord = ckMalloc(sizeof(double)*numElem);

	int i, j;
	for(i = 0; i <rows ; i++) {
		treecpy = tree;
		memcpy(nextCoord, coord, sizeof(double)*numElem);
		for (j = 0 ; j < cols; j++) {
			fscanf(f,"%lf,",&myvariable);
			dataPoints[j] = myvariable;
	    }
		
		while(treecpy->dir != -1){
			dir = treecpy->dir;
			midPoint= cutHalf(nextCoord, dir, dim);
			if(dataPoints[dir] > midPoint){
				nextCoord[dir] = midPoint;
				treecpy = treecpy->right;
			} else {
				nextCoord[dir+dim] = midPoint;
				treecpy = treecpy->left;
			}
		}
		if(dataPoints[dim] == treecpy->type){
			
			count++;			
		}
	}
	fclose(f);
	free(dataPoints);
	free(nextCoord);
	return (double)count/rows;
} 

//takes information from all the files and prints out data
void printData(char *testFile, char *outFile, treeNode tree){
	int dim = getCols(testFile)-1;
	double *initialRect = ckMalloc(sizeof(double)*dim*2);
	FILE* outputFile = fopen(outFile,"w");
	if(outputFile == NULL){
		fprintf(stderr,"Could not open output.txt: %s\n", strerror(errno));
		exit(1);
	}
	
	//set initial coordinates;
	int i;
	for(i=0; i < dim; i++){
		initialRect[i] = 0.0;
	}
	for(i=dim; i < dim*2; i++){
		initialRect[i]=1.0;
	}
	
	printPartitions(outputFile, tree, dim, initialRect);
	fclose(outputFile);
	
	double accuracy = reportAccuracy(testFile, tree, dim, initialRect);
	fprintf(stdout, "The accuracy is: %lf\n", accuracy);
	
	free(initialRect);
}

int main(int argc, char *argv[]){
	int lambda = atoi(argv[3]);
	int level = atoi(argv[4]);
	treeNode tree = trainData(argv[1],lambda,level);
	printData(argv[2], argv[5], tree);
	
	freeTree(tree);
	return 0;
}

