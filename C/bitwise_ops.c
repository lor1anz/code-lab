#include <stdio.h>

unsigned setbits(unsigned x, int n, int p, unsigned y)
{
    return ((~(~0 << n) & y) << (p - n + 1)) | (((~0 << (p + 1)) | ~(~0 << (p - n + 1))) & x);
}

int main(void)
{
    unsigned x = 011; /*1011*/
    unsigned y = 006; /*0110*/
    int n = 2;
    int p = 2;

    printf("%d\n", setbits(x, n, p, y)); /*1101 == 13*/

    return 0;
}
