// mem_test.c
#include <stdio.h>
#include <stdlib.h>
#include <gem5/m5ops.h>
int main() {
    m5_reset_stats(0, 0);

    size_t size = 1024*1024*50; // 50MB
    char *buffer = malloc(size);
    if(!buffer) return -1;

    for(size_t i=0; i<size; i++) {
        buffer[i] = i % 256;
    }

    long sum = 0;
    for(size_t i=0; i<size; i++) {
        sum += buffer[i];
    }

    m5_dump_stats(0, 0);
    printf("Memory sum: %ld\n", sum);
    free(buffer);
    return 0;
}
