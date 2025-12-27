#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

static struct nlist *hashtable[HASHSIZE];

unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
int undef(char *name);
void print_record(struct nlist *np);
void print_table();

int main(int argc, char const *argv[])
{
    struct nlist *np;

    install("111", "aaa");
    install("222", "bbb");
    install("777", "ccc");
    install("&&&", "ddd");

    printf("Initial table:\n");

    print_table();

    printf("\nLookup \"111\":\n");

    np = lookup("111");
    print_record(np);

    printf("\nLookup \"333\":\n");

    np = lookup("333");
    print_record(np);

    printf("\nUndef \"111\"\n");

    undef("111");
    print_table();

    printf("\nLookup \"111\":\n");

    np = lookup("111");
    print_record(np);

    return 0;
}

unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
    struct nlist *np;

    for (np = hashtable[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(np->name, s) == 0)
            return np;
    }
    return NULL;
}

struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtable[hashval];
        hashtable[hashval] = np;
    } else {
        free((void*)np->defn);
    }
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

int undef(char *name) {
    struct nlist *np;
    struct nlist *prev;
    unsigned hashval;

    if ((np = lookup(name)) == NULL)
        return 1;

    hashval = hash(name);
    prev = hashtable[hashval];
    if (prev == np) {
        hashtable[hashval] = np->next;
    } else {
        while ((prev++)->next != np);
        prev->next = np->next;
    }

    free((void *)np->name);
    free((void *)np->defn);
    free((void *)np);
    return 0;
}

void print_record(struct nlist *np) {
    if (np)
        printf("hash: %d, name: %s, defn: %s\n", hash(np->name), np->name, np->defn);
    else
        printf("Not found\n");
}

void print_table() {
    size_t i;
    struct nlist *np;

    for (i = 0; i < HASHSIZE; ++i) {
        if ((np = hashtable[i]) != NULL)
            print_record(np);
    }
}
