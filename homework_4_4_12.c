#include <stdio.h>
#include <string.h>

int main() {
    int hash[256] = {0};
    char raw[256], new[256];
    gets(raw);
    int p = 0;
    for (int i = 0; i < strlen(raw); i++) {
        if (hash[raw[i]]) continue;
        hash[raw[i]] = 1;
        new[p++] = raw[i];
    }
    puts(new);
    return 0;
}