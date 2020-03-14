#include <stdio.h>
#include <string.h>

char str[256];

int is_rev() {
    int len = strlen(str);
    for (int i = 0, j = len-1; i < j; i++, j--)
        if (str[i] != str[j]) return 0;
    return 1;
}

int main() {
    gets(str);
    if (is_rev())
        printf("Yes");
    else
        printf("No");
    return 0;
}