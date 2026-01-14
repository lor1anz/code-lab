#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096

void error(char *fmt, ...);

int main(int argc, char const *argv[])
{
    int src, dst, n;
    char buf[BUFSIZE];
    struct stat st;
    
    if (argc != 3)
        error("cp: missing file operand\n");
    if (stat(argv[1], &st) == -1)
        error("cp: can't stat file %s\n", argv[1]);
    if ((src = open(argv[1], O_RDONLY, 0)) == -1)
        error("cp: can't open file %s\n", argv[1]);
    if ((dst = creat(argv[2], st.st_mode & 0777)) == -1)
        error("cp: can't create file %s\n", argv[2]);

    while ((n = read(src, buf, BUFSIZE)) > 0)
        if (write(dst, buf, n) != n)
            error("cp: can't write to file %s\n", argv[2]);

    return 0;
}

void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
