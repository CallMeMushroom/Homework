#include <stdio.h>

int hash[1000000] = {0};
int n, num;

int main() {
    scanf("%d", &n);
    while (n--) {
        scanf("%d", &num);
        if (hash[num]) continue;
        hash[num] = 1;
        printf("%d ", num);
    }
    return 0;
}