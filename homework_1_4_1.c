#include <stdio.h>

int main(){
	float a, b, c;
	scanf("%f %f %f", &a, &b, &c);
	
	float max = a;
	if (max < b) max = b;
	if (max < c) max = c;
	
	printf("%f", max);	
}
