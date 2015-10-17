#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N 800
#define NC 3

#define RED 0
#define GREEN 1
#define BLUE 2

#define MAX_COLOR_VALUE 255

int colors[N][N][NC];

void drawRect(double input[], int colorIndex) {
  // adjust for coordinate system
  // first dimension represents the y coordinate
  // second dimension represents the x coordinate
  // (0,0) in rectangle space corresponds to (N-1,0)
  // in pixel space.
  int lbx = (int) floor(input[0]*N);
  int lby = N - (int) floor(input[3]*N);
  int ubx = (int) floor(input[2]*N);
  int uby = N - (int) floor(input[1]*N);

  int i,j;
  // -1 to get a black edge around the box
  for (i=lby; i < uby-1; i++) {
    for (j=lbx; j < ubx-1; j++) {
      colors[i][j][colorIndex] = MAX_COLOR_VALUE;
    }
  }
}


void initColors() {
  int i,j,k;
  for (i=0; i < N; i++) { 
    for (j=0; j < N; j++) {
      for (k=0; k < NC; k++) {
	colors[i][j][k] = 0;
      }
    }
  }
}


void printPPM() {
  printf("P3\n");
  printf("%d %d\n", N, N);
  printf("%d\n", MAX_COLOR_VALUE);
  
  int i,j,k;

  for (i=0; i < N; i++) {
    for (j=0; j < N; j++) {
      for (k=0; k < NC; k++) {
	printf("%d ", colors[i][j][k]);
      }
      printf("\n");
    }
  }
}


int readRect(int m, double rect[]) {
  // expected format lbx lby ubx uby color
  int i;
  for (i=0; i < m; i++) {
    int v = scanf("%lf", rect+i);
    if (v == EOF) 
      return 0;
    if (v != 1) {
      fprintf(stderr, "Bad format\n");
      exit(0);
    }
  }
  return 1;
}


int main(int argc, char *argv[]) {
  double input[5];

  initColors();

  while (readRect(5, input) != 0) {
    if (input[4] == RED) 
      drawRect(input, RED);
    else
      drawRect(input, BLUE);

  }
  printPPM();
  return 0;
}
