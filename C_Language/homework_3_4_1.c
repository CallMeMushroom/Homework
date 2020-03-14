#include <stdio.h>

int main(){
    char ch;
    ch = getchar();
    ch += 6;
    if ((ch > 'z') || ((ch > 'Z') && (ch < 'a')))
        ch -= 26;
    printf("%c", ch);
}