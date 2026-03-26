#include <stdio.h>
#include <stdlib.h>

int main() {

    char result[100];
    char cmd[256];
    char buffer[256];

    FILE *random;
    FILE *jsonc;
    
    sprintf(cmd, "jsonc.exe 123 treasure shipwreck bastion fortress");


    jsonc  = popen(cmd, "r");
    random = popen("random.exe", "r");

    if (jsonc == NULL) {
        printf("Gagal menjalankan command\n");
        return 1;
    }
    else if (random  == NULL) {
        printf("Gagal menjalankan command\n");
        return 1;
    }

    fgets(result, sizeof(result), random);

    printf("Hasil dari random.exe: %s \n", result);
    int line = 0 ;
    while (fgets(buffer, sizeof(buffer), jsonc)) {
        line++;

        if (line == 5) {
            printf("TARGET: %s", buffer);
            break;
        }
    }

    pclose(jsonc);
    pclose(random);

    return 0;
}

