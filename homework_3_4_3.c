#include <stdio.h>

int main(){
    int year;
    scanf("%d", &year);

    int wedn = 5;
    wedn += (year - 2019);
    wedn += (year - 2016) / 4;
    wedn -= (year - 2000) / 100;
    wedn += (year - 2000) / 400;
    wedn = (wedn - 1) % 7 + 1;
    if (wedn < 0) wedn += 7;

    printf("%d", wedn);
}