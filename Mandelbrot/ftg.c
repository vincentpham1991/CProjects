#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932385
void header(void) {
    printf("%%!PS-Adobe-3.0 EPSF-3.0\n");
    printf("%%%%BoundingBox: 0 0 532 532\n");
    printf("10 10 translate\n");
    printf("0 0 512 512 rectstroke\n");
}

void footer(void) {
    printf("showpage\n");
}

int main(void) {
    char command;               // input turtle command
    double d;                   // change in distance
    double a;                   // change in angle

    double r = 0.0;             // red   component of RGB color
    double g = 0.0;             // green component of RGB color
    double b = 0.0;             // blue  component of RGB color

    double x = 0.0;             // x coordinate of turtle
    double y = 0.0;             // y coordinate of turtle
    double alpha = 0.0;         // direction of turtle in degrees

    header();

// read in turtle graphics commands and process
while (scanf(" %c", &command) == 1) {
    // space in " %c" eats up any preceding whitespace
    switch(command) {

        // change color to r, g, b
        case 'C': scanf("%lf %lf %lf", &r, &g, &b);
                  printf("%f %f %f setrgbcolor\n", r, g, b);
                  break;

        // fly to location (x, y)
        case 'F': scanf("%lf %lf", &x, &y);
                  break;

        // draw spot of size d
        case 'S': scanf("%lf", &d);
                  printf("%f %f %f %f rectfill\n", x - d/2, y - d/2, d, d);
                  break; 

        // rotate
        case 'R': scanf("%lf", &a);
                  alpha += a;
                  break;

        // move d units in current direction, pen down
        case 'D': scanf("%lf", &d);
                  printf("%f %f moveto\n", x, y);
                  x += d * cos((PI/180.0) * alpha);
                  y += d * sin((PI/180.0) * alpha);
                  printf("%f %f lineto\n", x, y);
                  printf("stroke\n");
                  break;

        default:  printf("Illegal input format.\n");
                  exit(EXIT_FAILURE);

        }  // end switch 
    }      // end while 



    footer();

    return 0;  
}
