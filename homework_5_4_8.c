#include <stdio.h>
#include <string.h>

char str[32767], *p = str;

int is_letter(char x){
    if ('a' <= x && x <= 'z') return 1;
    if ('A' <= x && x <= 'Z') return 1;
    return 0;
}

int main() {
    gets(str);
    int cnt = is_letter(str[0]);
    int is_word = 1;
    while (p < str + strlen(str)) {
        if (!(is_word ^ is_letter(*p++))) continue;
        is_word = !is_word;
        cnt += is_word;
    }
    printf("%d", cnt);
    return 0;
}