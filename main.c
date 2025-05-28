#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

char odgovor;

int main() {
	int n = 0;

	while (1) {
		printf("1.Dodaj glazbu.\n");
		printf("2.Ispisi svu glazbu.\n");
		printf("3.Uredi glazbu.\n");
		printf("4.Pretrazi po imenu pjesme.\n");
		printf("5.Pretrazi po imeni izvodaca.\n");
		printf("6.Pretrazi po imenu albuma.\n");
		printf("7.Izbrisi glazbu.\n");
		printf("8.Izbrisi datoteku.\n");
		printf("9.Spremi i izadi.\n");

		do {
			printf("Unesite broj:");
			scanf("%d", &n);
			getchar();
		} while (!(n > 0 && n < 10));

		izbornik(n);

	}

	return 0;
}
