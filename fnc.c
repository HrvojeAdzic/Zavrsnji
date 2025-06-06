
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
	FILE* fp = NULL;
	ALBUM* tfp = (ALBUM*)malloc(sizeof(ALBUM));
	if (!tfp) {
		perror("Greska.");
		return;
	}

	printf("Unesite ime izvodaca:");
	scanf(" %19[^\n]", tfp->izvodac);
	printf("Unesite ime pjesme:");
	scanf(" %19[^\n]", tfp->pjesma);
	printf("Unesite ime albuma:");
	scanf(" %19[^\n]", tfp->album);

	fp = fopen("album.txt", "a");
	if (!fp) {
		perror("Greska pri otvaranju datoteke.");
		SAFE_FREE(tfp);
		return;
	}

	fprintf(fp, "Izvodac: %s\nPjesma: %s\nAlbum: %s\n\n", tfp->izvodac, tfp->pjesma, tfp->album);

	fclose(fp);
	SAFE_FREE(tfp);
	printf("Pjesma uspjesno dodana!\n");
}

void prntMsc() {
	int i = 0;
	FILE* fp = NULL;
	ALBUM* tfp;

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);
	if (brojGlazbe == 0) {
		printf("\nAlbum je prazan.\n");
		return;
	}


	tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("\nGreska.\n");
		return 1;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);

	sortiranje(tfp);

	if (brojGlazbe == 1) {
		printf("Postoji %d. spremljena pjesma.", brojGlazbe);
	}
	else {
		printf("Postoje %d. spremljene pjesme.", brojGlazbe);
	}


	for (i = 0; i < brojGlazbe; i++) {
		printf("\n\nPjesma:%s", (tfp + i)->pjesma);
		printf("\nIzvodac:%s", (tfp + i)->izvodac);
		printf("\nAlbum:%s", (tfp + i)->album);
	}

	printf("\n\n");

	fclose(fp);
	free(tfp);
}

void editMsc() {
	FILE* fp = NULL;
	ALBUM* tfp = NULL;
	int index = 0;
	int nadeno = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("Greska.\n");
		return 1;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);

	printf("Unesi ime pjesme za uredivanje:");
	scanf(" %19[^\n]", trazeno);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((tfp + i)->pjesma, trazeno) == 0) {
			index = i;
			nadeno = 1;
			break;
		}
	}

	if (nadeno == 0) {
		printf("\nPjesma nije nadena.\n\n");
		return;
	}
	else if (nadeno == 1) {
		printf("\nPjesma je nadena.\n\n");
		fp = fopen("album.bin", "rb+");
		if (fp == NULL) {
			perror("Greska.\n");
			return 1;
		}

		fread(&brojGlazbe, sizeof(int), 1, fp);
		rewind(fp);
		fseek(fp, 4, SEEK_SET);

		for (int i = 0; i < brojGlazbe; i++) {
			if (i == index) {
				printf("Unesite novo ime izvodaca:");
				scanf(" %19[^\n]", (tfp + i)->izvodac);
				printf("Unesite novo ime pjesme:");
				scanf(" %19[^\n]", (tfp + i)->pjesma);
				printf("Unesite novo ime albuma:");
				scanf(" %19[^\n]", (tfp + i)->album);

				fwrite((tfp + i), sizeof(ALBUM), 1, fp);
				break;
			}
			fseek(fp, sizeof(ALBUM), SEEK_CUR);
		}
	}

	fclose(fp);
	free(tfp);
}

int comparePjesma(const void* a, const void* b) {
	return strcmp(((ALBUM*)a)->pjesma, ((ALBUM*)b)->pjesma);
}

void srchSng() {
	FILE* fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	ALBUM* tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("Greska.");
		fclose(fp);
		return;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);

	char trazeno[20];
	printf("Unesite ime pjesme: ");
	scanf(" %19[^\n]", trazeno);

	qsort(tfp, brojGlazbe, sizeof(ALBUM), comparePjesma);

	ALBUM kljuc;
	strcpy(kljuc.pjesma, trazeno);

	ALBUM* rez = bsearch(&kljuc, tfp, brojGlazbe, sizeof(ALBUM), comparePjesma);
	if (rez == NULL) {
		printf("Pjesma nije nadena.\n\n");
		free(tfp);
		return;
	}

	int indeks = rez - tfp;

	while (indeks > 0 && strcmp(tfp[indeks - 1].pjesma, trazeno) == 0) {
		indeks--;
	}

	int br = 0;
	while (indeks < brojGlazbe && strcmp(tfp[indeks].pjesma, trazeno) == 0) {
		br++;
		printf("\nPjesma: %s", tfp[indeks].pjesma);
		printf("\nIzvodac: %s", tfp[indeks].izvodac);
		printf("\nAlbum: %s\n", tfp[indeks].album);
		indeks++;
	}

	printf("\nPjesma je %d puta nadena!\n\n", br);
	free(tfp);
}

int compareIzvodac(const void* a, const void* b) {
	return strcmp(((ALBUM*)a)->izvodac, ((ALBUM*)b)->izvodac);
}


void srchArt() {
	FILE* fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	ALBUM* tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("Greska.");
		fclose(fp);
		return;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);

	char trazeno[20];
	printf("Unesite ime izvodaca: ");
	scanf(" %19[^\n]", trazeno);

	qsort(tfp, brojGlazbe, sizeof(ALBUM), compareIzvodac);

	ALBUM kljuc;
	strcpy(kljuc.izvodac, trazeno);

	ALBUM* rez = bsearch(&kljuc, tfp, brojGlazbe, sizeof(ALBUM), compareIzvodac);
	if (rez == NULL) {
		printf("Izvodac nije naden.\n\n");
		free(tfp);
		return;
	}

	int indeks = rez - tfp;

	while (indeks > 0 && strcmp(tfp[indeks - 1].izvodac, trazeno) == 0) {
		indeks--;
	}

	int br = 0;
	while (indeks < brojGlazbe && strcmp(tfp[indeks].izvodac, trazeno) == 0) {
		br++;
		printf("\nIzvodac: %s", tfp[indeks].izvodac);
		printf("\nPjesma: %s", tfp[indeks].pjesma);
		printf("\nAlbum: %s\n", tfp[indeks].album);
		indeks++;
	}

	printf("\nIzvodac je %d puta naden!\n\n", br);
	free(tfp);
}

int compareAlbum(const void* a, const void* b) {
	return strcmp(((ALBUM*)a)->album, ((ALBUM*)b)->album);
}

void srchAlb() {
	FILE* fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	ALBUM* tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("Greska.");
		fclose(fp);
		return;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);

	char trazeno[20];
	printf("Unesite ime albuma: ");
	scanf(" %19[^\n]", trazeno);

	qsort(tfp, brojGlazbe, sizeof(ALBUM), compareAlbum);

	ALBUM kljuc;
	strcpy(kljuc.album, trazeno);

	ALBUM* rez = bsearch(&kljuc, tfp, brojGlazbe, sizeof(ALBUM), compareAlbum);
	if (rez == NULL) {
		printf("Album nije naden.\n\n");
		free(tfp);
		return;
	}

	int indeks = rez - tfp;

	while (indeks > 0 && strcmp(tfp[indeks - 1].album, trazeno) == 0) {
		indeks--;
	}

	int br = 0;
	while (indeks < brojGlazbe && strcmp(tfp[indeks].album, trazeno) == 0) {
		br++;
		printf("\nAlbum: %s", tfp[indeks].album);
		printf("\nPjesma: %s", tfp[indeks].pjesma);
		printf("\nIzvodac: %s\n", tfp[indeks].izvodac);
		indeks++;
	}

	printf("\nAlbum je %d puta naden!\n\n", br);
	free(tfp);
}

void delMsc() {
	ALBUM* tfp = NULL;
	FILE* fp = NULL;
	int index = 0;
	int nadeno = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	tfp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (tfp == NULL) {
		perror("Greska.");
		return 1;
	}

	fread(tfp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);
	printf("Unesite ime pjesme koju zelite obrisati:");
	scanf(" %19[^\n]", trazeno);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((tfp + i)->pjesma, trazeno) == 0) {
			index = i;
			nadeno = 1;
			break;
		}
	}

	if (nadeno == 0) {
		printf("\nTrazena pjesma nije nadena.\n\n");
		return;
	}

	fclose(fp);

	fp = fopen("album.bin", "wb");
	if (fp == NULL) {
		perror("Greska.");
		return 1;
	}

	brojGlazbe--;
	fwrite(&brojGlazbe, sizeof(int), 1, fp);
	for (int i = 0; i < brojGlazbe + 1; i++) {
		if (i == index) {
			continue;
		}
		else {
			fwrite((tfp + i), sizeof(ALBUM), 1, fp);
		}
	}
	printf("\nPjesma je uspjesno obrisana.\n\n");

	free(tfp);
	fclose(fp);
}

void delDat() {
	int odgovor = NULL;
	int x;
	char fp[] = "album.bin";
	printf("Jeste li sigurni da zelite obrisati datoteku (y/n):");

	while (1) {
		scanf("%c", &odgovor);
		if ((odgovor == 'y') || (odgovor == 'n')) {
			break;
		}
		printf("Krivi unos:");
		scanf("%c", &odgovor);
	}

	if (odgovor == 'Y') {
		x = remove(fp);
		if (x == 0) {
			printf("\nDatoteka uspjesno obrisana.\n\n");
			return;
		}
		else {
			printf("\nDatoteka ne uspjesno obrisana.\n\n");
			return;
		}
	}

	if (odgovor == 'N') {
		return 0;
	}
}


void izadji() {
	int odgovor = NULL;
	printf("Jeste li sigurni da zelite izaci iz programa (y/n):");

	while (1) {
		scanf("%c", &odgovor);
		if ((odgovor == 'y') || (odgovor == 'n')) {
			break;
		}
		printf("Krivi unos:");
		scanf("%c", &odgovor);
	}

	if (odgovor == 'y') {
		exit(0);
	}
	else {
		return 0;
	}
}

void sortiranje(ALBUM* copy) {
	if (!copy) return;

	qsort(copy, brojGlazbe, sizeof(ALBUM), compSng);
}

int compSng(const void* m, const void* n) {
	const ALBUM* albumM = (const ALBUM*)m;
	const ALBUM* albumN = (const ALBUM*)n;
	return strcmp(albumM->pjesma, albumN->pjesma);
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
