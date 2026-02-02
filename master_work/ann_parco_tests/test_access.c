#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <gem5/m5ops.h> // 要新增此 header file
int main(int argc, char *argv[])
{
        int i, num, random=30000;
        char string[10] = "./";
        char filename[10];
        num = atoi(argv[1]);
        srand(num);
        for (i=0; i<num; i++) {
                sprintf(filename, "%d", rand()%random);
                strcat(string, filename);
                printf("M5 reset...\n");
                printf("%d: access(%s, F_OK)\n", i+1, string);
                m5_reset_stats(0, 0);        // m5ops
                access(string, F_OK);
                m5_dump_stats(0, 0);         // m5ops
                printf("M5 dumped\n");
                memset(filename, '\0', 10);
                memset(string, '\0', 10);
                strcpy(string, "./");
        }

        return 0;
}