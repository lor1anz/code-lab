#include <fcntl.h>
#include <unistd.h>

// #define NULL    0
#define EOF     (-1)
#define BUFSIZ  1024
#define OPEN_MAX 20    /* max #files open at once */
#define PERMS 0666

typedef struct _iobuf {
    int   cnt;    /* characters left */
    char *ptr;    /* next character position */
    char *base;   /* location of buffer */
    int   flag;   /* mode of file access */
    int   fd;     /* file descriptor */
} FILE;

FILE _iob[OPEN_MAX];

enum _flags {
    _READ   = 01,    /* file open for reading */
    _WRITE  = 02,    /* file open for writing */
    _UNBUF  = 04,    /* file is unbuffered */
    _EOF    = 010,   /* EOF has occurred on this file */
    _ERR    = 020    /* error occurred on this file */
};

FILE *my_fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    
    for (fp = _iob; fp < _iob + OPEN_MAX; ++fp)
        if ((fp->flag & (_READ | _WRITE)) == 0)
            break;

    if (fp >= _iob + OPEN_MAX)
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else
        fd = open(name, O_RDONLY, 0);

    if (fd == -1)
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

int main(int argc, char const *argv[])
{
    FILE *file;
    char buf[BUFSIZ];
    int n;

    file = my_fopen("fopen.c", "r");
    if (!file)
        return 1;

    while ((n = read(file->fd, buf, BUFSIZ)) > 0)
        if (write(1, buf, n) == -1)
    
    return 0;
}
