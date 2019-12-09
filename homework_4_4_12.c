#include <stdio.h>
#include <string.h>

int main() {
    int hash[255] = {0};
    char raw[255], new[255];
    gets(raw);
    int len = strlen(raw);
    int p = 0;
    for (int i = 0; i < len; i++) {
        if (hash[raw[i]]) continue;
        hash[raw[i]] = 1;
        new[p++] = raw[i];
    }
    puts(new);
    return 0;
}