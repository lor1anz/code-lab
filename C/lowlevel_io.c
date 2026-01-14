#include <sys/unistd.h>
#include <stdio.h>

#define BUFSIZE 4096

void copy_input_to_output(void)
{
    char buf[BUFSIZE];
    int n;

    while ((n = read(0, buf, BUFSIZE)) > 0)
        write(1, buf, n);
}

int my_getchar(void)
{
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;

    if (n == 0) {
        n = read(0, buf, BUFSIZE);
        bufp = buf;
    }
    return (--n >= 0) ? *bufp++ : -1;
}

int main(void)
{
    char c;

    while ((c = my_getchar()) != -1) {
        printf("%c", c);
    }

    copy_input_to_output();

    return 0;
}
