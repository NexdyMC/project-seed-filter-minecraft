// gcc command.c -o cmd
#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>

#include <string.h>

int main(int argc, char * argv[])
{
    FILE * fprandom;
    FILE * fpjsonc;

    char random[100];
    char jsonc[200];
    char cmd[300];


    fprandom = popen("random.exe", "r");

    if (fprandom == NULL) {
        printf("Gagal menjalankan random.exe\n");
        return 1;
    }

    fgets(random, sizeof(random), fprandom);

    random[strcspn(random, "\n")] = 0;

    printf("Seed: %s\n", random);

    for (int mask = 1; mask < (1 << n); mask++) {
        sprintf(cmd, "jsonc.exe %s", seed);

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                strcat(cmd, " ");
                strcat(cmd, structures[i]);
            }
        }

        printf("RUN: %s\n", cmd);
        system(cmd);
    }

    // sprintf(cmd, "jsonc.exe %s treasure shipwreck bastion fortress", random);

    fpjsonc = popen(cmd, "r");

    while (fgets(jsonc, sizeof(jsonc), fpjsonc)) {
        printf("%s", jsonc);
    }

    pclose(fprandom);
    pclose(fpjsonc);

    return 0;
}