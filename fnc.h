#pragma once
#pragma once

typedef struct {
	char izvodac[20];
	char pjesma[20];
	char album[20];
}ALBUM;

void izbornik(int);
void addMsc();
void editMsc();
void prntMsc();
void srchSng();
void srchArt();
void srchAlb();
void delMsc();
void delDat();
void izadji();
void sortiranje(ALBUM*);
void preimenujDatoteku();
int compSng(const void* m, const void* n);

extern char odgovor;
