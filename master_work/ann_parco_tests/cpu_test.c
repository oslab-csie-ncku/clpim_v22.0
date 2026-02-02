// cpu_test.c
#include <stdio.h>
#include <gem5/m5ops.h>
int main() {
    printf("M5 CPU test start\n");
    m5_reset_stats(0, 0);

    long sum = 0;
    for (long i = 0; i < 100000000; i++) {
        sum += i;
    }

    m5_dump_stats(0, 0);
    printf("Sum: %ld\n", sum);
    printf("M5 CPU test end\n");
    return 0;
}
