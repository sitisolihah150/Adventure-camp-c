
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TRANSAKSI 100
#define JUMLAH_ALAT 5

typedef struct {
    char nama[50];
    int harga;
} Peralatan;

typedef struct {
    char namaPenyewa[100];
    int jumlahUnit[JUMLAH_ALAT];
    int lamaSewa;
    long long subtotal;
    long long diskon;
    long long totalBayar;
    int persentaseDiskon;
} Transaksi;

/* Data peralatan dan harga sewa per hari */
Peralatan alat[JUMLAH_ALAT] = {
    {"Tenda Kapasitas 4 Orang", 50000},
    {"Sleeping Bag", 15000},
    {"Kompor Portable", 20000},
    {"Matras Camping", 10000},
    {"Lampu Camping", 12000}
};

/* Array untuk menyimpan riwayat transaksi */
Transaksi riwayat[MAX_TRANSAKSI];
int jumlahTransaksi = 0;

/* Membersihkan sisa input */
void bersihkanBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Input angka dengan validasi */
int inputBilangan(const char *pesan, int minimum, int maksimum) {
    char baris[100];
    char sisa;
    int nilai;
    int hasil;

    while (1) {
        printf("%s", pesan);

        if (fgets(baris, sizeof(baris), stdin) == NULL) {
            printf("Input tidak valid. Coba lagi.\n");
            continue;
        }

        if (strchr(baris, '\n') == NULL && !feof(stdin)) {
            bersihkanBuffer();
            printf("Input terlalu panjang. Coba lagi.\n");
            continue;
        }

        hasil = sscanf(baris, " %d %c", &nilai, &sisa);

        if (hasil == 1 &&
            nilai >= minimum &&
            nilai <= maksimum) {
            return nilai;
        }

        printf("Masukkan angka antara %d sampai %d.\n",
               minimum, maksimum);
    }
}

/* Input teks dengan validasi */
void inputTeks(const char *pesan, char *teks, int ukuran) {
    size_t panjang;

    while (1) {
        printf("%s", pesan);

        if (fgets(teks, ukuran, stdin) == NULL) {
            teks[0] = '\0';
            continue;
        }

        panjang = strlen(teks);

        if (panjang > 0 && teks[panjang - 1] == '\n') {
            teks[panjang - 1] = '\0';
        } else {
            bersihkanBuffer();
        }

        if (strlen(teks) > 0) {
            return;
        }

        printf("Nama tidak boleh kosong.\n");
    }
}

/* Menampilkan daftar peralatan */
void tampilkanPeralatan(void) {
    int i;

    printf("\n");
    printf("========================================================\n");
    printf("                    DAFTAR PERALATAN\n");
    printf("========================================================\n");
    printf("%-4s %-32s %15s\n",
           "No", "Nama Peralatan", "Harga/Hari");
    printf("--------------------------------------------------------\n");

    for (i = 0; i < JUMLAH_ALAT; i++) {
        printf("%-4d %-32s Rp%12d\n",
               i + 1, alat[i].nama, alat[i].harga);
    }

    printf("========================================================\n");
}

/* Menghitung persentase diskon berdasarkan lama sewa */
int hitungDiskon(int lamaSewa) {
    if (lamaSewa >= 5) {
        return 15;
    } else if (lamaSewa >= 3) {
        return 10;
    }

    return 0;
}

/* Menampilkan struk penyewaan */
void tampilkanStruk(const Transaksi *t) {
    int i;

    printf("\n");
    printf("========================================================\n");
    printf("                    ADVENTURE CAMP\n");
    printf("                     STRUK PENYEWAAN\n");
    printf("========================================================\n");

    printf("Nama Penyewa : %s\n", t->namaPenyewa);
    printf("--------------------------------------------------------\n");
    printf("%-32s %5s %12s\n",
           "Peralatan", "Unit", "Biaya");

    for (i = 0; i < JUMLAH_ALAT; i++) {
        if (t->jumlahUnit[i] > 0) {
            long long biaya;

            biaya = (long long)alat[i].harga *
                    t->jumlahUnit[i] * t->lamaSewa;

            printf("%-32s %5d Rp%10lld\n",
                   alat[i].nama,
                   t->jumlahUnit[i],
                   biaya);
        }
    }

    printf("--------------------------------------------------------\n");
    printf("Lama Sewa      : %d hari\n", t->lamaSewa);
    printf("Subtotal       : Rp%lld\n", t->subtotal);
    printf("Diskon (%d%%)   : Rp%lld\n",
           t->persentaseDiskon, t->diskon);
    printf("--------------------------------------------------------\n");
    printf("TOTAL BAYAR    : Rp%lld\n", t->totalBayar);
    printf("========================================================\n");
    printf("Terima kasih telah menggunakan layanan Adventure Camp!\n");
}

/* Proses penyewaan peralatan */
void sewaPeralatan(void) {
    Transaksi t = {0};
    int i;
    int totalUnit = 0;
    int konfirmasi;
    char pilihan;

    if (jumlahTransaksi >= MAX_TRANSAKSI) {
        printf("\nKapasitas transaksi sudah penuh!\n");
        printf("Transaksi baru tidak dapat dilakukan.\n");
        return;
    }

    printf("\n");
    printf("========================================================\n");
    printf("                     SEWA PERALATAN\n");
    printf("========================================================\n");

    inputTeks("Nama penyewa: ",
              t.namaPenyewa,
              sizeof(t.namaPenyewa));

    tampilkanPeralatan();

    printf("\nMasukkan jumlah unit yang ingin disewa.\n");
    printf("Masukkan 0 jika tidak ingin menyewa barang tersebut.\n\n");

    for (i = 0; i < JUMLAH_ALAT; i++) {
        char pesan[120];

        snprintf(pesan, sizeof(pesan),
                 "Jumlah %s: ", alat[i].nama);

        t.jumlahUnit[i] = inputBilangan(pesan, 0, 1000);
        totalUnit += t.jumlahUnit[i];
    }

    if (totalUnit == 0) {
        printf("\nPenyewaan dibatalkan karena belum ada barang dipilih.\n");
        return;
    }

    t.lamaSewa = inputBilangan(
        "Lama penyewaan (hari): ", 1, 365
    );

    /* Menghitung subtotal seluruh peralatan */
    t.subtotal = 0;

    for (i = 0; i < JUMLAH_ALAT; i++) {
        t.subtotal += (long long)alat[i].harga *
                      t.jumlahUnit[i] *
                      t.lamaSewa;
    }

    /* Menghitung diskon dan total pembayaran */
    t.persentaseDiskon = hitungDiskon(t.lamaSewa);
    t.diskon = t.subtotal * t.persentaseDiskon / 100;
    t.totalBayar = t.subtotal - t.diskon;

    tampilkanStruk(&t);

    while (1) {
        printf("\nSimpan transaksi? (Y/T): ");

        if (scanf(" %c", &pilihan) != 1) {
            bersihkanBuffer();
            continue;
        }

        bersihkanBuffer();
        pilihan = (char)toupper((unsigned char)pilihan);

        if (pilihan == 'Y') {
            riwayat[jumlahTransaksi] = t;
            jumlahTransaksi++;

            printf("\nTransaksi berhasil disimpan!\n");
            break;
        } else if (pilihan == 'T') {
            printf("\nTransaksi tidak disimpan.\n");
            break;
        } else {
            printf("Pilihan tidak valid. Masukkan Y atau T.\n");
        }
    }
}

/* Menampilkan riwayat transaksi dengan format rapi */
void tampilkanRiwayat(void) {
    int i, j;
    int adaPeralatan;
    long long totalPendapatan = 0;

    printf("\n");
    printf("============================================================\n");
    printf("                    RIWAYAT TRANSAKSI\n");
    printf("============================================================\n");

    if (jumlahTransaksi == 0) {
        printf("          Belum ada transaksi penyewaan.\n");
        printf("============================================================\n");
        return;
    }

    printf("%-4s %-22s %-12s %14s\n",
           "No", "Nama Penyewa", "Lama Sewa", "Total Bayar");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < jumlahTransaksi; i++) {
        printf("%-4d %-22.22s %-12d Rp%11lld\n",
               i + 1,
               riwayat[i].namaPenyewa,
               riwayat[i].lamaSewa,
               riwayat[i].totalBayar);

        printf("     Peralatan:\n");
        adaPeralatan = 0;

        for (j = 0; j < JUMLAH_ALAT; j++) {
            if (riwayat[i].jumlahUnit[j] > 0) {
                printf("       - %-32s (%d unit)\n",
                       alat[j].nama,
                       riwayat[i].jumlahUnit[j]);

                adaPeralatan = 1;
            }
        }

        if (!adaPeralatan) {
            printf("       Tidak ada peralatan.\n");
        }

        printf("------------------------------------------------------------\n");

        totalPendapatan += riwayat[i].totalBayar;
    }

    printf("Jumlah Transaksi : %d\n", jumlahTransaksi);
    printf("Total Pendapatan : Rp%lld\n", totalPendapatan);
    printf("============================================================\n");
}

/* Menu utama program */
int main(void) {
    int menu;
    int berjalan = 1;

    do {
        printf("\n");
        printf("========================================================\n");
        printf("                    ADVENTURE CAMP\n");
        printf("              SISTEM PENYEWAAN PERALATAN\n");
        printf("========================================================\n");
        printf("1. Lihat Daftar Peralatan\n");
        printf("2. Sewa Peralatan\n");
        printf("3. Lihat Riwayat Transaksi\n");
        printf("4. Keluar\n");
        printf("========================================================\n");

        menu = inputBilangan("Pilih menu [1-4]: ", 1, 4);

        switch (menu) {
            case 1:
                tampilkanPeralatan();
                break;

            case 2:
                sewaPeralatan();
                break;

            case 3:
                tampilkanRiwayat();
                break;

            case 4:
                printf("\nTerima kasih telah menggunakan\n");
                printf("Sistem Penyewaan Adventure Camp!\n");
                berjalan = 0;
                break;
        }

    } while (berjalan);

    return 0;
}
