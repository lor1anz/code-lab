#include <stdio.h>
#include <stdarg.h>

void my_fprintf(FILE *file, char *fmt, ...)
{
    va_list ap;
    char *p;
    char *s_val;
    int i_val;
    double d_val;

    va_start(ap, fmt);
    for (p = fmt; *p; ++p) {
        if (*p != '%') {
            putc(*p, file);
            continue;
        }
        switch (*++p) {
            case 'd':
                i_val = va_arg(ap, int);
                fprintf(file, "%d", i_val);
                break;
            case 'f':
                d_val = va_arg(ap, double);
                fprintf(file, "%f", d_val);
                break;
            case 's':
                s_val = va_arg(ap, char *);
                fprintf(file, "%s", s_val);
                break;
            default:
                putc(*p, file);
        }
    }

    va_end(ap);
}


int main(void)
{
    int i_val = 1;
    double d_val = 1.7;
    char *s_val = "string_val";
    my_fprintf(stdout, "Hello, it's my fprintf with vals: %d, %f, %s\n", i_val, d_val, s_val);
    my_fprintf(stdout, "But with no vals like %e or %g (see source code)\n", d_val, d_val);
    my_fprintf(stdout, "Just trying some var args\n");
    return 0;
}
