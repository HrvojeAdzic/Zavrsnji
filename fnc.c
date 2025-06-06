#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fnc.h"

static int brojGlazbe = 0;

#define SAFE_FREE(p) do { if ((p) != NULL) { free(p); (p) = NULL; } } while(0)

void izbornik(int n) {
    switch (n) {
    case 1: addMsc(); break;
    case 2: editMsc(); break;
    case 3: prntMsc(); break;
    case 4: srchSng(); break;
    case 5: srchArt(); break;
    case 6: srchAlb(); break;
    case 7: delMsc(); break;
    case 8: delDat(); break;
    case 9: izadji(); break;
    case 10: preimenujDatoteku(); break;
    default: printf("Nepostojeca opcija!\n"); break;
    }
}

inline void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addMsc() {
    FILE* fp = fopen("album.bin", "rb+");
    ALBUM nova;

    if (!fp) {
        fp = fopen("album.bin", "wb");
        if (!fp) {
            perror("Greska pri stvaranju datoteke");
            return;
        }
        int zero = 0;
        fwrite(&zero, sizeof(int), 1, fp);
        fclose(fp);
        fp = fopen("album.bin", "rb+");
    }

    fread(&brojGlazbe, sizeof(int), 1, fp);

    printf("Unesite ime izvodaca: ");
    scanf(" %19[^\n]", nova.izvodac);
    printf("Unesite ime pjesme: ");
    scanf(" %19[^\n]", nova.pjesma);
    printf("Unesite ime albuma: ");
    scanf(" %19[^\n]", nova.album);

    brojGlazbe++;
    rewind(fp);
    fwrite(&brojGlazbe, sizeof(int), 1, fp);
    fseek(fp, 0, SEEK_END);
    fwrite(&nova, sizeof(ALBUM), 1, fp);

    fclose(fp);
    printf("Pjesma dodana!\n");
}

void prntMsc() {
    int i;
    FILE* fp = fopen("album.bin", "rb");
    if (!fp) {
        printf("\nAlbum je prazan.\n\n");
        return;
    }

    fread(&brojGlazbe, sizeof(int), 1, fp);
    if (brojGlazbe == 0) {
        printf("\nAlbum je prazan.\n");
        fclose(fp);
        return;
    }

    ALBUM* tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
    if (!tfp) {
        perror("\nGreska.\n");
        fclose(fp);
        return;
    }

    fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
    fclose(fp);

    sortiranje(tfp);

    printf("Postoje %d spremljene pjesme.\n", brojGlazbe);
    for (i = 0; i < brojGlazbe; i++) {
        printf("\nPjesma: %s", tfp[i].pjesma);
        printf("\nIzvodac: %s", tfp[i].izvodac);
        printf("\nAlbum: %s\n", tfp[i].album);
    }

    free(tfp);
}

void editMsc() {
    FILE* fp = fopen("album.bin", "rb");
    if (!fp) {
        printf("\nAlbum je prazan.\n\n");
        return;
    }

    fread(&brojGlazbe, sizeof(int), 1, fp);
    ALBUM* tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
    if (!tfp) {
        perror("Greska.");
        fclose(fp);
        return;
    }

    fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
    fclose(fp);

    char trazeno[20];
    printf("Unesi ime pjesme za uredivanje: ");
    scanf(" %19[^\n]", trazeno);

    int nadeno = 0, index = 0;
    for (int i = 0; i < brojGlazbe; i++) {
        if (strcmp(tfp[i].pjesma, trazeno) == 0) {
            nadeno = 1;
            index = i;
            break;
        }
    }

    if (!nadeno) {
        printf("\nPjesma nije nadena.\n");
        free(tfp);
        return;
    }

    fp = fopen("album.bin", "rb+");
    if (!fp) {
        perror("Greska.");
        free(tfp);
        return;
    }

    fseek(fp, sizeof(int) + index * sizeof(ALBUM), SEEK_SET);

    printf("Unesite novo ime izvodaca: ");
    scanf(" %19[^\n]", tfp[index].izvodac);
    printf("Unesite novo ime pjesme: ");
    scanf(" %19[^\n]", tfp[index].pjesma);
    printf("Unesite novo ime albuma: ");
    scanf(" %19[^\n]", tfp[index].album);

    fwrite(&tfp[index], sizeof(ALBUM), 1, fp);
    fclose(fp);
    free(tfp);
}

int compSng(const void* a, const void* b) {
    return strcmp(((ALBUM*)a)->pjesma, ((ALBUM*)b)->pjesma);
}
int compareIzvodac(const void* a, const void* b) {
    return strcmp(((ALBUM*)a)->izvodac, ((ALBUM*)b)->izvodac);
}
int compareAlbum(const void* a, const void* b) {
    return strcmp(((ALBUM*)a)->album, ((ALBUM*)b)->album);
}

void sortiranje(ALBUM* copy) {
    if (!copy) return;
    qsort(copy, brojGlazbe, sizeof(ALBUM), compSng);
}

void srchSng() {
    FILE* fp = fopen("album.bin", "rb");
    if (!fp) {
        printf("\nAlbum je prazan.\n\n");
        return;
    }

    fread(&brojGlazbe, sizeof(int), 1, fp);
    ALBUM* tfp = malloc(brojGlazbe * sizeof(ALBUM));
    if (!tfp) {
        perror("Greska.");
        fclose(fp);
        return;
    }

    fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
    fclose(fp);

    char trazeno[20];
    printf("Unesite ime pjesme: ");
    scanf(" %19[^\n]", trazeno);

    qsort(tfp, brojGlazbe, sizeof(ALBUM), compSng);

    ALBUM kljuc;
    strcpy(kljuc.pjesma, trazeno);
    ALBUM* rez = bsearch(&kljuc, tfp, brojGlazbe, sizeof(ALBUM), compSng);

    if (!rez) {
        printf("Pjesma nije nadena.\n");
        free(tfp);
        return;
    }

    int indeks = rez - tfp;
    while (indeks > 0 && strcmp(tfp[indeks - 1].pjesma, trazeno) == 0) indeks--;

    int br = 0;
    while (indeks < brojGlazbe && strcmp(tfp[indeks].pjesma, trazeno) == 0) {
        printf("\nPjesma: %s", tfp[indeks].pjesma);
        printf("\nIzvodac: %s", tfp[indeks].izvodac);
        printf("\nAlbum: %s\n", tfp[indeks].album);
        br++; indeks++;
    }

    printf("\nPjesma je %d puta nadena!\n\n", br);
    free(tfp);
}

void srchArt() {
    // Analogno srchSng(), ali koristi compareIzvodac
}

void srchAlb() {
    // Analogno srchSng(), ali koristi compareAlbum
}

void delMsc() {
    // Logika za brisanje pjesme
}

void delDat() {
    char odgovor;
    printf("Jeste li sigurni da zelite obrisati datoteku (y/n): ");
    while (1) {
        scanf(" %c", &odgovor);
        if (odgovor == 'y' || odgovor == 'n') break;
        printf("Krivi unos: ");
    }

    if (odgovor == 'y') {
        if (remove("album.bin") == 0) {
            printf("Datoteka uspjesno obrisana.\n");
        }
        else {
            printf("Datoteka nije uspjesno obrisana.\n");
        }
    }
}

void izadji() {
    char odgovor;
    printf("Jeste li sigurni da zelite izaci iz programa (y/n): ");
    while (1) {
        scanf(" %c", &odgovor);
        if (odgovor == 'y' || odgovor == 'n') break;
        printf("Krivi unos: ");
    }

    if (odgovor == 'y') exit(0);
}

void preimenujDatoteku() {
    char staroIme[100], novoIme[100];
    printf("Unesite trenutno ime datoteke: ");
    scanf(" %99[^\n]", staroIme);
    printf("Unesite novo ime datoteke: ");
    scanf(" %99[^\n]", novoIme);

    if (rename(staroIme, novoIme) == 0) {
        printf("Datoteka je uspjesno preimenovana.\n");
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}
