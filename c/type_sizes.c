#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void check_malloc(unsigned nbytes);

int main(void)
{
    printf("------Void-------\n");
    printf("\n");

    printf("void %ld\n", sizeof(void));

    printf("\n\n");
    printf("------Integer-------\n");
    printf("\n");

    printf("char %ld\n", sizeof(char));
    printf("short %ld\n", sizeof(short));
    printf("int %ld\n", sizeof(int));
    printf("long %ld\n", sizeof(long));
    printf("long long %ld\n", sizeof(long long));

    printf("\n");

    printf("signed char min %d\n", CHAR_MIN);
    printf("signed char max %d\n", CHAR_MAX);
    printf("signed short min %d\n", SHRT_MIN);
    printf("signed short max %d\n", SHRT_MAX);
    printf("signed int min %d\n", INT_MIN);
    printf("signed int max %d\n", INT_MAX);
    printf("signed long min %ld\n", LONG_MIN);
    printf("signed long max %ld\n", LONG_MAX);
    printf("signed long long min %lld\n", LLONG_MIN);
    printf("signed long long max %lld\n", LLONG_MAX);

    printf("\n");

    printf("signed char max %d\n", UCHAR_MAX);
    printf("signed short max %d\n", USHRT_MAX);
    printf("signed int max %d\n", UINT_MAX);
    printf("signed long max %ld\n", ULONG_MAX);
    printf("signed long long max %lld\n", ULLONG_MAX);

    printf("\n\n");
    printf("------Float------\n");
    printf("\n");

    printf("float %ld\n", sizeof(float));
    printf("double %ld\n", sizeof(double));
    printf("long double %ld\n", sizeof(long double));

    printf("\n");

    printf("float dig %d\n", FLT_DIG);
    printf("double dig %d\n", DBL_DIG);
    printf("long double dig %d\n", LDBL_DIG);

    printf("\n\n");
    printf("----Malloc----\n");
    printf("\n");
    
    check_malloc(1);
    check_malloc(3);
    check_malloc(4);
    check_malloc(7);
    check_malloc(8);
    check_malloc(11);
    check_malloc(32);
    check_malloc(50);

    printf("\n");

    return 0;
}

void check_malloc(unsigned nbytes)
{
    void *mem = malloc(nbytes * sizeof(char));
    printf("bytes requested %2u: %lu\n", nbytes, malloc_usable_size(mem));
    free(mem);
}
