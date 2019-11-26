#include <stdio.h>

int main() {
    int n, p = 0, cnt[50] = {0};
    for (int i = 10; i < 32; i++) { // 32*32 > 1000
        n = i*i;
        if (n / 100 == n % 10 ||
            n / 100 == n / 10 % 10 ||
            n % 10 == n / 10 % 10) cnt[p++] = n;
    }
    printf("%d\n", p);
    while (p--) printf("%d ", cnt[p]);
    return 0;
}