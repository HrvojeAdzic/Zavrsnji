
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

static int brojGlazbe = 0;

void izbornik(int n) {

	switch (n) {
	case 1:
		dodajGlazbu();
		break;

	case 2:
		ispisGlazbe();
		break;

	case 3:
		urediGlazbu();
		break;

	case 4:
		pretrazivanjePjesme();
		break;

	case 5:
		pretrazivanjeIzvodaca();
		break;

	case 6:
		pretrazivanjeAlbuma();
		break;

	case 7:
		brisanjeGlazbe();
		break;

	case 8:
		brisanjeDatoteke();
		break;

	case 9:
		izlaz();
		break;

	}
}

void dodajGlazbu() {
	FILE* fp = NULL;
	ALBUM* temp = NULL;

	temp = (ALBUM*)malloc(sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.\n");
		return 1;
	}

	printf("Unesite ime izvodaca:");
	scanf(" %19[^\n]", temp->izvodac);
	printf("Unesite ime pjesme:");
	scanf(" %19[^\n]", temp->pjesma);
	printf("Unesite ime albuma:");
	scanf(" %19[^\n]", temp->album);

	fp = fopen("album.bin", "rb");

	if (fp == NULL) {
		fp = fopen("album.bin", "wb");
		brojGlazbe++;
		fwrite(&brojGlazbe, sizeof(int), 1, fp);
		fwrite(temp, sizeof(ALBUM), 1, fp);
		fclose(fp);
	}
	else {
		fclose(fp);
		fp = fopen("album.bin", "rb+");
		if (fp == NULL) {
			perror("Greska.\n");
			return;
		}
		else {
			rewind(fp);
			fread(&brojGlazbe, sizeof(int), 1, fp);
			brojGlazbe++;
			rewind(fp);
			fwrite(&brojGlazbe, sizeof(int), 1, fp);
			fseek(fp, 0, SEEK_END);
			fwrite(temp, sizeof(ALBUM), 1, fp);
			fclose(fp);
		}
	}

	fclose(fp);
	free(temp);
}

void ispisGlazbe() {
	int i = 0;
	FILE* fp = NULL;
	ALBUM* temp;

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);
	if (brojGlazbe == 0) {
		printf("\nAlbum je prazan.\n\n");
		return;
	}


	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("\nGreska.\n");
		return 1;
	}

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);

	sortiranje(temp);

	if (brojGlazbe == 1) {
		printf("Postoji %d. spremljena pjesma.", brojGlazbe);
	}
	else {
		printf("Postoje %d. spremljene pjesme.", brojGlazbe);
	}


	for (i = 0; i < brojGlazbe; i++) {
		printf("\n\nPjesma:%s", (temp + i)->pjesma);
		printf("\nIzvodac:%s", (temp + i)->izvodac);
		printf("\nAlbum:%s", (temp + i)->album);
	}

	printf("\n\n");

	fclose(fp);
	free(temp);
}

void urediGlazbu() {
	FILE* fp = NULL;
	ALBUM* temp = NULL;
	int index = 0;
	int nadeno = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.\n");
		return 1;
	}

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);

	printf("Unesi ime pjesme za uredivanje:");
	scanf(" %19[^\n]", trazeno);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((temp + i)->pjesma, trazeno) == 0) {
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
				scanf(" %19[^\n]", (temp + i)->izvodac);
				printf("Unesite novo ime pjesme:");
				scanf(" %19[^\n]", (temp + i)->pjesma);
				printf("Unesite novo ime albuma:");
				scanf(" %19[^\n]", (temp + i)->album);

				fwrite((temp + i), sizeof(ALBUM), 1, fp);
				break;
			}
			fseek(fp, sizeof(ALBUM), SEEK_CUR);
		}
	}

	fclose(fp);
	free(temp);
}

void pretrazivanjePjesme() {
	FILE* fp = NULL;
	ALBUM* temp = NULL;
	int index = 0;
	int nadeno = 0;
	int br = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.");
		return 1;
	}

	printf("Unesite ime pjesme:");
	scanf(" %19[^\n]", trazeno);

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((temp + i)->pjesma, trazeno) == 0) {
			br++;
			if (br == 1) {
				printf("\nPjesma je 1 put nadena!\n");
			}
			else {
				printf("\nPjesma je %d puta nadena!\n", br);
			}
			printf("\nPjesma:%s", (temp + i)->pjesma);
			printf("\nIzvodac:%s", (temp + i)->izvodac);
			printf("\nAlbum:%s\n", (temp + i)->album);

			nadeno = 1;
		}
	}

	printf("\n");

	if (nadeno == 0) {
		printf("Pjesma nije nadena.\n\n");
	}

	fclose(fp);
	free(temp);
}

void pretrazivanjeIzvodaca() {
	FILE* fp = NULL;
	ALBUM* temp = NULL;
	int index = 0;
	int nadeno = 0;
	int br = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.");
		return 1;
	}

	printf("Unesite ime izvodaca:");
	scanf(" %19[^\n]", trazeno);

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((temp + i)->izvodac, trazeno) == 0) {
			br++;
			if (br == 1) {
				printf("\nIzvodac je 1 put naden!\n");
			}
			else {
				printf("\nIzvodac je %d puta naden!\n", br);
			}
			printf("\nIzvodac:%s", (temp + i)->izvodac);
			printf("\nPjesma:%s", (temp + i)->pjesma);
			printf("\nAlbum:%s\n", (temp + i)->album);

			nadeno = 1;
		}
	}

	printf("\n");

	if (nadeno == 0) {
		printf("Izvodac nije naden.\n\n");
	}

	fclose(fp);
	free(temp);
}

void pretrazivanjeAlbuma() {
	FILE* fp = NULL;
	ALBUM* temp = NULL;
	int index = 0;
	int nadeno = 0;
	int br = 0;
	char trazeno[20];

	fp = fopen("album.bin", "rb");
	if (fp == NULL) {
		printf("\nAlbum je prazan.\n\n");
		return 1;
	}

	fread(&brojGlazbe, sizeof(int), 1, fp);

	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.");
		return 1;
	}

	printf("Unesite ime albuma:");
	scanf(" %19[^\n]", trazeno);

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((temp + i)->album, trazeno) == 0) {
			br++;
			if (br == 1) {
				printf("\nAlbum je 1 put naden!\n");
			}
			else {
				printf("\n\nAlbum je %d puta naden!\n", br);
			}
			printf("\nAlbum:%s", (temp + i)->album);
			printf("\nPjesma:%s", (temp + i)->pjesma);
			printf("\nIzvodac:%s", (temp + i)->izvodac);

			nadeno = 1;
		}
	}

	printf("\n\n");

	if (nadeno == 0) {
		printf("Album nije naden.\n\n");
	}

	fclose(fp);
	free(temp);
}

void brisanjeGlazbe() {
	ALBUM* temp = NULL;
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

	temp = (ALBUM*)malloc(brojGlazbe * sizeof(ALBUM));
	if (temp == NULL) {
		perror("Greska.");
		return 1;
	}

	fread(temp, sizeof(ALBUM), brojGlazbe, fp);
	fclose(fp);
	printf("Unesite ime pjesme koju zelite obrisati:");
	scanf(" %19[^\n]", trazeno);

	for (int i = 0; i < brojGlazbe; i++) {
		if (strcmp((temp + i)->pjesma, trazeno) == 0) {
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
			fwrite((temp + i), sizeof(ALBUM), 1, fp);
		}
	}
	printf("\nPjesma je uspjesno obrisana.\n\n");

	free(temp);
	fclose(fp);
}

void brisanjeDatoteke() {
	odgovor = NULL;
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

void izlaz() {
	odgovor = NULL;
	printf("Jeste li sigurni da zelite izaci iz programa (y/n):");

	while (1) {
		scanf("%c", &odgovor);
		if ((odgovor == 'y') || (odgovor == 'n')) {
			break;
		}
		printf("Krivi unos:");
		scanf("%c", &odgovor);
	}

	if (odgovor == 'Y') {
		exit(0);
	}
	else {
		return 0;
	}
}

void sortiranje(ALBUM* copy) {
	if (copy == NULL) {
		return;
	}

	qsort(copy, brojGlazbe, sizeof(ALBUM), comparePjesma);
}

int comparePjesma(const void* a, const void* b) {
	const ALBUM* albumA = (const ALBUM*)a;
	const ALBUM* albumB = (const ALBUM*)b;
	return strcmp(albumA->pjesma, albumB->pjesma);
}
