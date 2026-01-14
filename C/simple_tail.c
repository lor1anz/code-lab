#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_lines(int n, char const *file_name)
{
    FILE *input;
    size_t len = 0;
    size_t read = 0;
    int counter = 0;
    int i;
    char *line = NULL;
    char **lines = (char**)malloc(n * sizeof(char*));

    input = fopen(file_name, "r");
    if (input == NULL) {
        printf("No such file: \"%s\"\n", file_name);
        return;
    }
    
    while ((read = getline(&lines[counter++ % n], &len, input)) != EOF);

    for (i = (counter > n ? counter - 1 : 0); i < (counter > n ? counter + n - 1 : counter); ++i) {
        printf("%s", lines[i % n]);
    }

    free(lines);
    fclose(input);
}

int main(int argc, char const *argv[])
{
    int n = 10;
    char const *file_name;
    
    if (argc < 2 || argc > 4) {
        printf("Usage: [OPTION]... [FILE]...\n");
        return 0;
    }

    short is_option = 0;
    for (int i = 1; i < argc; ++i) {
        if (is_option) {
            n = atoi(argv[i]);
            is_option = 0;
        } else if (!strcmp(argv[i], "-n")) {
            is_option = 1;
        } else {
            file_name = argv[i];
        }
    }

    print_lines(n, file_name);

    return 0;
}