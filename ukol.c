#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// dopredna deklarace
 typedef struct uzel uzel;

// Struktura pro data
typedef struct
{
	// datove polozky
	char	nazev[30];
	char 	typ[15];
	char	cislo[20];
	char 	odpovedny[25];
	char	datum[15];
	char 	kontrola[25];
	char	stav[15];
}data_typ;

struct uzel
{
	// datove polozky
	data_typ* data;
	// ukazatel na nasledujici prvek
	uzel* naslednik;
};

typedef struct
{
	uzel* zacatek;
	uzel* konec;
}spojovy_seznam;

// funkce pro nacteni jednoho radku textu ze souboru
bool fgetLine(FILE* soubor, char* string, int maxDelka)
{
    for (int i = 0; i < maxDelka; i++) {
        // precti znak z terminalu
        int znak = fgetc(soubor);

        // kontrola jestli nedoslo k chybe cteni
        if (ferror(soubor)) {
            printf("Chyba: neslo precist cely radek\n");
            return false;
        }

        // zkontroluj, jestli neni konec vstupu
        if (znak != EOF && znak != '\n')
            string[i] = znak;
        else {
            // konec vstupu - ukonci string a prerus cyklus
            string[i] = '\0';
            return true;
        }
    }

    // v cyklu jsme nedosli ke znaku EOF ani k '\n' - radek je delsi nez (maxDelka - 1)
    // (vypisujeme maxDelka - 1, protoze posledni znak musi byt '\0', ktery ale uzivatel nezadava)
    printf("Chyba: radek je delsi nez maximalni povolena delka %d znaku.\n", maxDelka - 1);
    return false;
}

void na_zacatek(spojovy_seznam* s, data_typ* data){
	uzel* uzl = vytvor_uzel(data);
	if (uzl == NULL){
		free(data);
	}

	if (s->zacatek != NULL){
		uzl->naslednik = s->zacatek;
		s->zacatek = uzl;
	} else {
		s->zacatek = uzl;
		s->konec = uzl;
	}
}

void vypis_seznam(spojovy_seznam* s){
	printf("\nVypis seznamu:\n");
	uzel* naslednik = s->zacatek;
	int i = 1;
	if(naslednik == NULL)
		printf("Prazdny seznam.\n");
	while(naslednik != NULL){
		vypis_data(naslednik->data);
		naslednik = naslednik->naslednik;
		i++;
	}
	printf("\n");
}

char* nacti_polozku(const char* string, int zacatek){
	if(zacatek == strlen(string)-1){
		char chyba[5];
		chyba = "CHYBA";
		return chyba;
	}

	for (int i = zacatek; i < strlen(string); i++){
		if (string[i] == 'i'){
			char polozka[i-zacatek];
			for(int j = 0, j < i-zacatek, j++){
				polozka[j] = string[zacatek + j];
			}
			return polozka;
		}
	}
	char polozka[strlen(string)-zacatek];
	for(int j = 0, j < strlen(string) - zacatek, j++){
		polozka[j] = string[zacatek + j];
		}
	return polozka;
}

int prazdna_polozka(char* polozka){
	for (int i = 0; i < strlen(polozka); i++){
		if (isspace(polozka[i])){
			return 1;
		}
	}
	return 0;
}

data_typ* zpracuj_radek(const char* string){
  int pocet_stredniku = 0
  for (size_t i = 0; i < strlen(string); i++) {
    if (string[i] == ';') {
      pocet_stredniku++;
    }
  }
  if (pocet_stredniku != 6) {
    return NULL;
  }

  data_typ* data = malloc(sizeof(data_typ));
  if(data == NULL){
    printf("chyba alokace\n");
    fclose(soubor);
    return 1;
  }
  // zpracovani radku
  int pocet_udaju = 0;

  char* nazev = nacti_polozky(string, 0);
  data->nazev = nazev;

  char* typ = nacti_polozky(string, strlen(nazev));
  data->typ = typ;

  char* cislo = nacti_polozky(string, strlen(typ));
  data->cislo = cislo;

  char* odpovedny = nacti_polozky(string, strlen(cislo));
  data->odpovedny = odpovedny;

  char* datum = nacti_polozky(string, strlen(odpovedny));
  data->datum = datum;

  char* kontrola = nacti_polozky(string, strlen(datum));
  data->kontrola = kontrola;

  char* stav = nacti_polozky(string, strlen(kontrola));
  data->stav = stav;

  retrun data;
}

void nacist_soubor(spojovy_seznam* s, FILE* soubor){
	while(!feof(soubor)){
		// Precteni radku
		char radek[1000];
		fgetLine(soubor, radek, 1000);
		// preskocit hlavicku
		if (radek[0] == '#')
			continue;
		// vytvoreni dat
		data_typ* data = malloc(sizeof(data_typ));
		if(data == NULL){
			printf("chyba alokace\n");
			fclose(soubor);
			return 1;
		}
		// zpracovani radku
		int pocet_udaju = 0;

		char* nazev = nacti_polozky(string, 0);
		data->nazev = nazev;

		char* typ = nacti_polozky(string, strlen(nazev));
		data->typ = typ;

		char* cislo = nacti_polozky(string, strlen(typ));
		data->cislo = cislo;

		char* odpovedny = nacti_polozky(string, strlen(cislo));
		data->odpovedny = odpovedny;

		char* datum = nacti_polozky(string, strlen(odpovedny));
		data->datum = datum;

		char* kontrola = nacti_polozky(string, strlen(datum));
		data->kontrola = kontrola;

		char* stav = nacti_polozky(string, strlen(kontrola));
		data->stav = stav;

		//vlozit data do seznamu
		na_zacatek(&s, data);

	}
}



int main(){

	return 0;
}
