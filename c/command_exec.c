#include <stdlib.h>

int main(void)
{
    system("date");
    system("ls -a");
    system("head -20 ./command_exec.c");
    system("echo \"Hello from C\"");
    system("grep \'echo\' ./command_exec.c");
    return 0;
}
