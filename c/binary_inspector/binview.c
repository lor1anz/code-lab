#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <elf.h>

#define CHUNK 16
#define BOX_WIDTH 38

enum errors 
{
    nofile = 1,
    badopts,
    fileopen,
    fileread
};

struct options {
    size_t max_lines;
    int show_all;
};

static struct options options;

static void box_line(void);
static void box_text(const char *s);
static void box_empty(void);

static int inspect_file(const char *file);
static void detect_elf(FILE *binfile);

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "%s: no file specified\n", argv[0]);
        return nofile;
    }

    options.max_lines = 5;
    options.show_all = 0;

    int opt;
    char seen_n = 0;
    while ((opt = getopt(argc, argv, "an:")) != -1) {
        switch (opt) {
            case 'a':
                options.show_all = 1;
                break;
            case 'n':
                options.max_lines = (size_t)strtoul(optarg, NULL, 10);
                seen_n = 1;
                break;
            default:
                fprintf(stderr, "%s: usage: [-a | -n N] file\n", argv[0]);
                return 1;
        }
    }

    if (options.show_all && seen_n > 0) {
        fprintf(stderr, "%s: options -a and -n are mutually exclusive\n", argv[0]);
        return badopts;
    }

    if (optind >= argc) {
        fprintf(stderr, "%s: no file specified\n", argv[0]);
        return nofile;
    }
    
    for (int i = optind; i < argc; ++i) {
        switch (inspect_file(argv[i])) {
            case fileopen:
                fprintf(stderr, "%s: can't open file \"%s\"\n", argv[0], argv[i]);
                return fileopen;
            case fileread:
                fprintf(stderr, "%s: can't read file \"%s\"\n", argv[0], argv[i]);
                return fileread;
        }
    }

    return 0;
}

static int inspect_file(const char *file)
{
    size_t i, n;
    size_t counter = 0;
    unsigned char buf[CHUNK];

    FILE *binfile = fopen(file, "rb");
    if (!binfile)
        return fileopen;

    detect_elf(binfile);
    
    fprintf(stdout, "%-8s  ", "Offset");
    fprintf(stdout, "%-48s ", "Hex bytes");
    fprintf(stdout, "%-18s ", "ASCII");
    fprintf(stdout, "\n");

    size_t max_lines = options.max_lines;
    while ((n = fread(buf, 1, CHUNK, binfile)) > 0) {
        if (!options.show_all && max_lines-- <= 0)
            break;

        /* offset */
        fprintf(stdout, "%08zx  ", counter);

        /* hex */
        for (i = 0; i < n; ++i) {
            fprintf(stdout, "%02x ", buf[i]);
        }

        /* ASCII */
        fprintf(stdout, " |");
        for (i = 0; i < n; ++i) {
            if (!isprint(buf[i]))
                fprintf(stdout, ".");
            else
                fprintf(stdout, "%c", buf[i]);
        }
        fprintf(stdout, "|");

        fprintf(stdout, "\n");
        counter += n;    
    }

    if (ferror(binfile)) {
        fclose(binfile);
        return fileread;
    }

    fclose(binfile);
    return 0;
}

static void detect_elf(FILE *binfile)
{
    Elf64_Ehdr eh;
    long prev_pos;
    unsigned char ident[CHUNK];
    char str_buf[BOX_WIDTH + 1];

    /* assuming seekable file */

    prev_pos = ftell(binfile);

    fseek(binfile, 0, SEEK_SET);
    if (fread(ident, 1, CHUNK, binfile) != CHUNK) {
        fseek(binfile, prev_pos, SEEK_SET);
        return;
    }

    int is_elf = 
        ident[0] == 0x7f &&
        ident[1] == 0x45 &&
        ident[2] == 0x4c &&
        ident[3] == 0x46;

    if (is_elf) {
        box_line();
        box_text("ELF Info");
        box_line();

        snprintf(str_buf, sizeof str_buf, "Class:      ELF%d", ident[4] == 2 ? 64 : 32);
        box_text(str_buf);

        snprintf(str_buf, sizeof str_buf, "Endianness: %s",
            ident[5] == 1 ? "little" :
            ident[5] == 2 ? "big" : "unknown");
        box_text(str_buf);

        box_empty();
        box_text("ELF Header");
        box_line();

        /* assuming native little-endian */

        fseek(binfile, 0, SEEK_SET);
        if (fread(&eh, 1, sizeof eh, binfile) == sizeof eh) {
            snprintf(str_buf, sizeof str_buf, "Type:      %u", eh.e_type);
            box_text(str_buf);

            snprintf(str_buf, sizeof str_buf, "Machine:   %u", eh.e_machine);
            box_text(str_buf);

            snprintf(str_buf, sizeof str_buf, "Entry:     0x%lx", (unsigned long)eh.e_entry);
            box_text(str_buf);

            box_line();
        }

        putchar('\n');
    }

    fseek(binfile, prev_pos, SEEK_SET);
}

static void box_line(void)
{
    putchar('+');
    for (int i = 0; i < BOX_WIDTH + 2; ++i)
        putchar('-');
    puts("+");
}

static void box_text(const char *s)
{
    printf("| %-*s |\n", BOX_WIDTH, s);
}

static void box_empty(void)
{
    box_text("");
}
