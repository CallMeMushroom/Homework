#include <stdio.h>
#include <math.h>

int isprime(int x){
	if (x == 1) return 0;
	for (int i = 2; i <= sqrt(x); i++)
		if (x % i == 0) return 0;
	return 1;
}

void main(){
	int num;
	scanf("%d", &num);
	isprime(num)? printf("Yes"): printf("No");
}
