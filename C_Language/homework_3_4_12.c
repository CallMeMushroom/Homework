#include <stdio.h>

int main() {
    for (int i = 1100; i > 1000; i--)
        if ((i % 3 == 2) &&
            (i % 5 == 4) &&
            (i % 7 == 6)) printf("%d", i);
    return 0;
}