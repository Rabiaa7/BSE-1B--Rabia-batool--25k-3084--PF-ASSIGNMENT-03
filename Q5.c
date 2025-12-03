// 25K-3084
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 4
#define BUFFER_CHUNK 512

typedef struct {
    char **lines;
    int size;
    int capacity;
} Buffer;

void initBuffer(Buffer *b) {
    b->lines = malloc(sizeof(char*) * INITIAL_CAPACITY);
    if (!b->lines) { perror("malloc"); exit(1); }
    b->size = 0;
    b->capacity = INITIAL_CAPACITY;
}

void freeAll(Buffer *b) {
    int i;
    for (i = 0; i < b->size; ++i) free(b->lines[i]);
    free(b->lines);
    b->lines = NULL;
    b->size = b->capacity = 0;
}

void ensureCapacity(Buffer *b, int minCap) {
    if (b->capacity >= minCap) return;
    int newCap = b->capacity * 2;
    while (newCap < minCap) newCap *= 2;
    char **tmp = realloc(b->lines, sizeof(char*) * newCap);
    if (!tmp) { perror("realloc"); freeAll(b); exit(1); }
    b->lines = tmp;
    b->capacity = newCap;
}

void insertLine(Buffer *b, int index, const char *text) {
    if (index < 0) index = 0;
    if (index > b->size) index = b->size;
    ensureCapacity(b, b->size + 1);

    if (index < b->size)
        memmove(&b->lines[index+1], &b->lines[index], sizeof(char*) * (b->size - index));

    size_t len = strlen(text);
    char *line = malloc(len + 1);

    if (!line) { perror("malloc"); freeAll(b); exit(1); }
    memcpy(line, text, len + 1);

    b->lines[index] = line;
    b->size++;
}

void deleteLine(Buffer *b, int index) {
    if (index < 0 || index >= b->size) {
        printf("Invalid index\n");
        return;
    }
    free(b->lines[index]);

    if (index < b->size - 1)
        memmove(&b->lines[index], &b->lines[index+1], sizeof(char*) * (b->size - 1 - index));

    b->size--;
}

void printAllLines(const Buffer *b) {
    int i;
    for (i = 0; i < b->size; ++i) {
        printf("%d: %s\n", i+1, b->lines[i]);
    }
}

void shrinkToFit(Buffer *b) {
    if (b->capacity == b->size) return;

    if (b->size == 0) {
        free(b->lines);
        b->lines = malloc(sizeof(char*) * INITIAL_CAPACITY);
        if (!b->lines) { perror("malloc"); exit(1); }
        b->capacity = INITIAL_CAPACITY;
        return;
    }

    char **tmp = realloc(b->lines, sizeof(char*) * b->size);
    if (!tmp) { perror("realloc"); return; }
    b->lines = tmp;
    b->capacity = b->size;
}

int saveToFile(const Buffer *b, const char *filename) {
    int i;
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen"); return -1; }

    for (i = 0; i < b->size; ++i) {
        if (fprintf(f, "%s\n", b->lines[i]) < 0) {
            perror("fprintf");
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return 0;
}

int loadFromFile(Buffer *b, const char *filename) {
    int i;
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); return -1; }

    for (i = 0; i < b->size; ++i) free(b->lines[i]);
    b->size = 0;

    char chunk[BUFFER_CHUNK];

    while (fgets(chunk, sizeof(chunk), f)) {
        size_t totalLen = strlen(chunk);
        char *line = malloc(totalLen + 1);
        if (!line) { perror("malloc"); fclose(f); return -1; }
        strcpy(line, chunk);

        while (line[totalLen-1] != '\n' && !feof(f)) {
            if (!fgets(chunk, sizeof(chunk), f)) break;
            size_t addLen = strlen(chunk);
            char *tmp = realloc(line, totalLen + addLen + 1);
            if (!tmp) { free(line); perror("realloc"); fclose(f); return -1; }
            line = tmp;
            memcpy(line + totalLen, chunk, addLen + 1);
            totalLen += addLen;
        }

        if (totalLen > 0 && line[totalLen-1] == '\n')
            line[totalLen-1] = '\0';

        insertLine(b, b->size, line);
        free(line);
    }

    fclose(f);
    return 0;
}

int main(void) {
    Buffer b;
    initBuffer(&b);
    int i;

    insertLine(&b, 0, "hi this is rabia");
    insertLine(&b, 1, "i am doing my pf assignment");
    insertLine(&b, 2, "its almost finished");

    printf("Buffer after inserts:\n");
    printAllLines(&b);

    deleteLine(&b, 1);
    printf("\nAfter deleting line 2:\n");
    printAllLines(&b);

    insertLine(&b, 1, "its a bit hard to do!");
    printf("\nAfter insert in middle:\n");
    printAllLines(&b);

    printf("\nSaving to file 'buffer.txt'...\n");
    if (saveToFile(&b, "buffer.txt") == 0) printf("Saved.\n");

    printf("\nClearing and reloading from file...\n");
    for (i = 0; i < b.size; ++i) free(b.lines[i]);
    b.size = 0;

    loadFromFile(&b, "buffer.txt");
    printAllLines(&b);

    printf("\nShrinking to fit...\n");
    shrinkToFit(&b);
    printf("Size = %d, Capacity = %d\n", b.size, b.capacity);

    freeAll(&b);
    return 0;
}
