#include <stdio.h>
#include <string.h>

char str[255], *p = str;

int main() {
    gets(str);
    int len = strlen(str);
    while (p < str + len){
        putchar(*p++);
        if (!((p-str) % 3)) putchar(' ');
    }
    return 0;
}