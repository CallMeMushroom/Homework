#include <stdio.h>

int small(int a, int b){
	if (a < b) return a;
	return b;
}

void main(){
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);

	printf("%d", small(a, small(b, c))); 
}