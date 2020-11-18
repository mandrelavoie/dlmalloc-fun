#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "dlmalloc/malloc.h"

#define PRINT_DEBUG_INFO 0

#define CHUNK_SIZE 0x200

static void display_chunk(char* name, uint8_t *ptr, int freed) {
    uint64_t *headers = (uint64_t*)ptr - 2;
    uint64_t prev_size = headers[0];
    uint64_t size_raw = headers[1];
    uint64_t size = size_raw & ~3;
    uint64_t prev_in_use = size_raw & 1;

    printf("Chunk %s [0x%lx]\n", name, (uint64_t)headers);
    printf("    prev_size:       0x%016lx   ", prev_size);
    printf("    size:            0x%016lx   ", size);
    printf("    prev_in_use:     %d\n", (char)prev_in_use);

    uint64_t *fd = headers[2];
    uint64_t *bk = headers[3];
    printf("    fd:              0x%016lx   ", fd);
    printf("    bk:              0x%016lx\n", bk);
    

    uint64_t *next_size = headers + (size / 8) + 1;
    uint64_t in_use = next_size[0] & 1;

    printf("    *data:        ");
    for (int i = 0; i < 24; i++) {
        printf("%02x ", ptr[i]);
    }
    printf("... ");
    for (int i = CHUNK_SIZE - 5; i < CHUNK_SIZE; i++) {
        printf("%02x ", ptr[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    char *a = malloc(CHUNK_SIZE);
    char *b = malloc(CHUNK_SIZE);
    char *c = malloc(CHUNK_SIZE);
    char *d = malloc(CHUNK_SIZE);

    if (argc > 1)
        strcpy(a, argv[1]);
    if (argc > 3)
        strcpy(c, argv[3]);
    if (argc > 2)
        strcpy(b, argv[2]);
    if (argc > 4)
        strcpy(d, argv[4]);

    #if PRINT_DEBUG_INFO
    printf("Before freeing:\n");
    display_chunk("a", a, 0);
    display_chunk("b", b, 0);
    display_chunk("c", c, 0);
    display_chunk("d", d, 0);
    #endif

    free(c);

    #if PRINT_DEBUG_INFO
    printf("\nAfter freeing b:\n");
    display_chunk("a", a, 0);
    display_chunk("b", b, 0);
    display_chunk("c", c, 1);
    display_chunk("d", d, 0);
    #endif

    free(b);

    #if PRINT_DEBUG_INFO
    printf("\nAfter freeing b:\n");
    display_chunk("a", a, 1);
    display_chunk("b", b, 0);
    display_chunk("c", c, 1);
    display_chunk("d", d, 0);
    #endif

    time_t now = time(0);

    printf("\nNope. Not exploited.\nBy the way... it's %s\n", ctime(&now));

    return 0;
}
