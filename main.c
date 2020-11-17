#include <stdio.h>
#include <string.h>

#include "dlmalloc/malloc.h"

#define CHUNK_SIZE 32

int main(int argc, char **argv)
{
    char *a = malloc(CHUNK_SIZE);
    char *b = malloc(CHUNK_SIZE);
    char *c = malloc(CHUNK_SIZE);

    if (argc > 1)
        strcpy(a, argv[1]);
    if (argc > 2)
        strcpy(b, argv[2]);
    if (argc > 3)
        strcpy(c, argv[3]);

    free(c);
    free(b);
    free(a);

    printf("Nope. Not exploited.\n");

    return 0;
}
