#include <stdio.h>
#include <string.h>

#define MAX_DATA 100   // kapasitas maksimum array

/* =========================================
   STRUCTURE
   Structure berisi:
   - nama  (string)
   - type  (integer)
========================================= */
typedef struct {
    char nama[50];
    int type;
} MapBiome;

/* =========================================
   ARRAY GLOBAL
   Array bertipe MapBiome
========================================= */
MapBiome Bmaps[MAX_DATA];  // array of struct
int count = 0;             // jumlah data aktif

/* =========================================
   CREATE
   Menambahkan data ke array
========================================= */
void createBiome() {

    if (count >= MAX_DATA) {
        printf("Data penuh!\n");
        return;
    }

    printf("Masukkan nama  : ");
    scanf("%s", Bmaps[count].nama);

    printf("Masukkan type  : ");
    scanf("%d", &Bmaps[count].type);

    count++; // tambah jumlah data aktif

    printf("Data berhasil ditambahkan!\n");
}

/* =========================================
   READ
   Menampilkan semua data
========================================= */
void readBiome() {

    if (count == 0) {
        printf("Belum ada data.\n");
        return;
    }

    printf("\n=== DATA LIST ===\n");

    for (int i = 0; i < count; i++) {
        printf("%d. Nama: %s | Type: %d\n",
               i,
               Bmaps[i].nama,
               Bmaps[i].type);
    }
}

/* =========================================
   UPDATE
   Mengubah data berdasarkan index
========================================= */
void updateBiome() {

    int index;

    if (count == 0) {
        printf("Belum ada data.\n");
        return;
    }

    readBiome();

    printf("Masukkan index yang ingin diubah: ");
    scanf("%d", &index);

    if (index < 0 || index >= count) {
        printf("Index tidak valid!\n");
        return;
    }

    printf("Masukkan nama baru : ");
    scanf("%s", Bmaps[index].nama);

    printf("Masukkan type baru : ");
    scanf("%d", &Bmaps[index].type);

    printf("Data berhasil diupdate!\n");
}

/* =========================================
   DELETE
   Menghapus data berdasarkan index
   Caranya: geser elemen ke kiri
========================================= */
void deleteBiome() {

    int index;

    if (count == 0) {
        printf("Belum ada data.\n");
        return;
    }

    readBiome();

    printf("Masukkan index yang ingin dihapus: ");
    scanf("%d", &index);

    if (index < 0 || index >= count) {
        printf("Index tidak valid!\n");
        return;
    }

    // Geser elemen setelah index ke kiri
    for (int i = index; i < count - 1; i++) {
        Bmaps[i] = Bmaps[i + 1];
    }

    count--; // kurangi jumlah data

    printf("Data berhasil dihapus!\n");
}

/* =========================================
   CLEAR ALL
   Menghapus semua data
   Cukup reset count ke 0
========================================= */
void clearAll() {

    count = 0;

    printf("Semua data berhasil dihapus!\n");
}

/* =========================================
   MAIN MENU
========================================= */
int main() {

    int pilihan;

    do {
        printf("\n===== MENU CRUD MAP BIOME =====\n");
        printf("1. Tambah Data\n");
        printf("2. Tampilkan Data\n");
        printf("3. Update Data\n");
        printf("4. Hapus Data\n");
        printf("5. Hapus Semua Data\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                createBiome();
                break;
            case 2:
                readBiome();
                break;
            case 3:
                updateBiome();
                break;
            case 4:
                deleteBiome();
                break;
            case 5:
                clearAll();
                break;
            case 0:
                printf("Program selesai.\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }

    } while (pilihan != 0);

    return 0;
}