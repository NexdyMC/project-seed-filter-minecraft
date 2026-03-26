#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main() {

    FILE *file = fopen("data.json", "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = 0;

    fclose(file);

    cJSON *json = cJSON_Parse(buffer);

    cJSON *nama = cJSON_GetObjectItem(json, "nama");
    cJSON *umur = cJSON_GetObjectItem(json, "umur");

    printf("Nama: %s\n", nama->valuestring);
    printf("Umur: %d\n", umur->valueint);

    cJSON_Delete(json);
    free(buffer);

    return 0;
}