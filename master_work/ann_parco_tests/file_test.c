// file_test.c
#include <stdio.h>
#include <stdlib.h>
#include <gem5/m5ops.h>
int main() {
    FILE *fp;
    char filename[20] = "testfile.txt";

    m5_reset_stats(0, 0);

    fp = fopen(filename, "w");
    for(int i = 0; i < 1000; i++) {
        fprintf(fp, "Line %d\n", i);
    }
    fclose(fp);

    fp = fopen(filename, "r");
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        // do nothing
    }
    fclose(fp);

    m5_dump_stats(0, 0);
    printf("File access test done\n");
    return 0;
}
