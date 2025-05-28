#pragma once

typedef struct {
	char izvodac[20];
	char pjesma[20];
	char album[20];
}ALBUM;

void izbornik(int);
void dodajGlazbu();
void urediGlazbu();
void ispisGlazbe();
void pretrazivanjePjesme();
void pretrazivanjeIzvodaca();
void pretrazivanjeAlbuma();
void brisanjeGlazbe();
void brisanjeDatoteke();
void izlaz();
void sortiranje(ALBUM*);
int comparePjesma(const void* a, const void* b);

extern char odgovor;



