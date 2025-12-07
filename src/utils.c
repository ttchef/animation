
#include <utils.h>

#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>

char* readFileToString(const char *path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed reading file: %s\n", path);
        return NULL;;
    }

    fseek(file, 0, SEEK_END);
    int32_t size = ftell(file);
    rewind(file);

    char* buf = malloc(size + 1);
    if (!buf) {
        fprintf(stderr, "Failed allocating for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    fread(buf, 1, size, file);
    buf[size] = '\0';

    fclose(file);
    
    return buf;
}

