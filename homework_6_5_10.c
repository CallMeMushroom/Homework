#include <stdio.h>

short a[100000];
int n;

void sort(short *num, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (*(num+i) > *(num+j)) {
                *(num+i) ^= *(num+j);
                *(num+j) ^= *(num+i);
                *(num+i) ^= *(num+j);
            }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    sort(a, n);
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    return 0;
}