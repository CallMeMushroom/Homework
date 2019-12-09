#include <stdio.h>
#include <math.h>
#include <string.h>

int prime(int x) {
    if (x == 2) return 2;
    for (int i = 2; i < abs(sqrt(x)+1) ;i++)
        if (x % i == 0) return 0;
    return x;
}

int main() {
    int total = 0;
    int n;
    scanf("%d", &n);
    for (int i = 2; i <= n; i++) total += prime(i);
    printf("%d ", total);
    return 0;
}