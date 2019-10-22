#include <stdio.h>
#include <math.h>

unsigned short isprime(int x) {
    for (int i = 2; i <= sqrt(x); i++)
        if (x % i == 0) return 0;
    return 1;

}

int main() {
    int cnt = 0;
    for (int i = 2; i <= 100; i++)
        if (isprime(i)) {
            printf("%d ", i);
            cnt++;
        }
    printf("\ntotal: %d", cnt);   
    return 0; 
}