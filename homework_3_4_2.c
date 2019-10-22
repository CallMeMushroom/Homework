#include <stdio.h>

int main(){
    float x;
    scanf("%f", &x);
    if      (x < 0 ) printf("%f", -x);
    else if (x < 10) printf("%f", x + 5);
    else if (x < 20) printf("%f", x + 10);
    else             printf("%f", x + 20);
}