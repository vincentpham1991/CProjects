/*
 * mandgray.c
 *
 *  Created on: Jan 20, 2013
 *      Author: Vincent
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int mandel(double x, double y){
	int n = 0;
	double r = x;
	double i = y;
	double r_prev = r;
	
	while((r*r +i*i) <= 4) {
		r = r*r - i*i + x;
		i = 2.0*r_prev*i + y;
		r_prev = r;
		n++;
	    //printf("r: %d, i: %d\n", r, i);
		if (n >= 255) {
			return 255;
		}
	}
	return n;
}


void scanner(void){
	int n;
	double xmin, ymin, width, height;
	/*
	n= 4;
	xmin = -1.5;
	ymin = -1.0;
	width = 2.0;
	height = 2.0;
	*/
	scanf("%d\n", &n);
	scanf("%lf\n", &xmin);
	scanf("%lf\n", &ymin);
	scanf("%lf\n", &width);
	scanf("%lf", &height);
	
	double xmax = xmin + width;
	double ymax = ymin + height;
	
	int t;
	double s_x, t_x, s_y, t_y, xTrans, yTrans, x_center1, y_center1, size, gs1;
	int count, i, j;
	s_x = 512/(xmax-xmin);
	t_x = 0 - xmin*s_x;
	s_y = 512/(ymax - ymin);
	t_y = 0 - ymin*s_y;
	
	
	x_center1 = 0;
	y_center1 = 0;
	
	size = (512/n);
	for(i = 0; i < n; i ++){
		x_center1 = (xmin+width/(n*2)) + i*(width/n);
		xTrans = x_center1*s_x + t_x;
		for(j = 0; j < n; j++){
			y_center1 = (ymin+height/(n*2)) + j*(height/n);
			
			yTrans = y_center1*s_y + t_y;
			t = mandel(x_center1, y_center1);
			gs1 = 1.0-t/255.0;
			
			//printf("xcenter: %1f , ycenter: %1f  t: %d\n ", x_center1, y_center1, t);
			
			printf("C %1f %1f %1f\n", gs1, gs1, gs1);
			printf("F %1f %1f\n", xTrans, yTrans);
			printf("S %1f\n\n", size);
			
		}
	}

}

int main(void){
	int red = mandel(.125,.75);
	int y = 4;
	/*
	printf ("hello there mate");
	*/
	//printf("%d\n",red);
	
	scanner();

	return 0;
}
