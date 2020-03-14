#include <stdio.h>
#include <string.h>

int main() {
    int hash[256] = {0};
    char raw[256], new[256];
    gets(raw);
    int len = strlen(raw);
    int p = 0;
    for (char *t = raw; t < raw + len; t++) {
        if (hash[*t]) continue;
        hash[*t] = 1;
        new[p++] = *t;
    }
    puts(new);
    return 0;
}