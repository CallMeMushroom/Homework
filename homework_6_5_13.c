#include <stdio.h>
#include <math.h>

short judge(int num) {
    int cnt = 1;
    for (int i = 2; i < sqrt(num); i++)
        if (num % i == 0)
            cnt += i + num / i;
    return num == cnt;
}

int n;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        if (judge(i))
            printf("%d ", i);
    return 0;
}