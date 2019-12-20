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
	char	datum[25];
	char 	kontrola[30];
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

uzel* vytvor_uzel(data_typ* data){
	uzel* uzl = malloc(sizeof(uzel));
	if(uzl == NULL){
		printf("chyba alokace\n");
		return NULL;
	}

	uzl->data = data;
	uzl->naslednik = NULL;

	return uzl;
}

void na_zacatek(spojovy_seznam* s, data_typ* data){
	uzel* uzl = vytvor_uzel(data);
	if (uzl == NULL){
		free(data);
	}
  printf("vytvoren uzel\n" );

	if (s->zacatek != NULL){
		uzl->naslednik = s->zacatek;
		s->zacatek = uzl;
	} else {
		s->zacatek = uzl;
		s->konec = uzl;
	}
}

void vypis_data(data_typ* data){
	printf("%s %s %s %s %s %s %s \n",
            data->nazev,
            data->typ,
            data->cislo,
            data->odpovedny,
            data->datum,
            data->kontrola,
            data->stav
        );
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

void zrus_seznam(spojovy_seznam* s)
{
	while (s->zacatek != NULL) {
		uzel* aktualni = s->zacatek;
		s->zacatek = aktualni->naslednik;
		free(aktualni->data);
		free(aktualni);
	}
	s->konec = NULL;
}

//--------------------------------------------------------------

void nacti_polozku(const char* string, int zacatek, char* polozka){
  int vytvoreno = 0;

  // vytvoreni polozky, pokud bude nalezen strednik
	for (int i = zacatek; i < strlen(string); i++){
		if (string[i] == ';'){
			for(int j = 0; j < i-zacatek; j++){
				polozka[j] = string[zacatek + j];
        if (j == i-zacatek - 1) {
          polozka[j+1] = '\0';
        }
			}
      vytvoreno = 1;
      break;
		}
	}
  // vytvoreni polozky, pokud nebyl nalezen strednik
  if(vytvoreno == 0){
    for(int j = 0; j < strlen(string) - zacatek; j++){
        polozka[j] = string[zacatek + j];
        if (j == strlen(string) - zacatek - 1) {
          polozka[j+1] = '\0';
        }
      }
  }
  printf("%s\n", polozka);
}

int prazdna_polozka(char* polozka){
	for (int i = 0; i < strlen(polozka); i++){
		if (isspace(polozka[i])){
			return 1;
		}
	}
	return 0;
}

data_typ* zpracuj_radek(const char* string, data_typ* data){
  /*
  Zpracuje radek, overi pocet udaju na radku (pomoci poctu stredniku) a vrati
  data_typ* naplneny polozkami z radku.
  */

  // kontrola poctu stredniku
  int pocet_stredniku = 0;
  for (size_t i = 0; i < strlen(string); i++) {
    if (string[i] == ';') {
      pocet_stredniku++;
    }
  }
  if (pocet_stredniku != 6) {
    return NULL;
  }

  int zacatek = 0;
  nacti_polozku(string, zacatek, data->nazev);
  zacatek += strlen(data->nazev)+1;
  printf("%s\n", data->nazev);

  nacti_polozku(string, zacatek, data->typ);
  zacatek += strlen(data->typ)+1;
  printf("%s\n", data->typ);

  nacti_polozku(string, zacatek, data->cislo);
  zacatek += strlen(data->cislo)+1;
  printf("%s\n", data->cislo);

  nacti_polozku(string, zacatek, data->odpovedny);
  zacatek += strlen(data->odpovedny)+1;
  printf("%s\n", data->odpovedny);

  nacti_polozku(string, zacatek, data->datum);
  zacatek += strlen(data->datum)+1;
  printf("%s\n", data->datum);

  nacti_polozku(string, zacatek, data->kontrola);
  zacatek += strlen(data->kontrola)+1;
  printf("%s\n", data->datum);
  printf("%s\n", data->kontrola);

  nacti_polozku(string, zacatek, data->stav);
  printf("%s\n", data->stav);
  printf("%s\n", data->datum);

  vypis_data(data);

  char	nazev[30];
	char 	typ[15];
	char	cislo[20];
	char 	odpovedny[25];
	char	datum[15];
	char 	kontrola[25];
	char	stav[15];

  return data;
}

//--------------------------------------------------------------
// TESTOVACI FUNKCE

int TEST_prazdny_string(char* string){
  /*
  Kontroluje, jestli je daný string prázdný, pokud ano, vrátí 0,
  pokud ne, vrátí 1;
  */
  for (size_t i = 0; i < strlen(string); i++) {
    if (isspace(string[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

int TEST_pocet_udaju(data_typ* data){
    return 1;
}

//--------------------------------------------------------------

void nacist_soubor(spojovy_seznam* s, FILE* soubor){
	while(!feof(soubor)){
		// Precteni radku
    printf("nacten radek\n");
		char radek[1000];
		fgetLine(soubor, radek, 1000);
		// preskocit hlavicku
		if (radek[0] == '#'){
      printf("preskocen prvni\n");
			continue;
      }
		// vytvoreni dat
    printf("neni prvni, vytvarim data\n");
		data_typ* data = malloc(sizeof(data_typ));
		if(data == NULL){
			printf("chyba alokace\n");
			fclose(soubor);
			return;
		}
    printf("data vytvorena, zpracovavam radek\n");
		// zpracovani radku
		data = zpracuj_radek(radek, data);
    printf("radek zpracovan\n");

    //vypis_data(data);
    printf("\n" );

		//vlozit data do seznamu
		na_zacatek(s, data);
    printf("radek vlozen do seznamu\n");

	}
}



int main(){

  FILE* soubor = fopen("test0.txt", "r");
	if (soubor == NULL) {
	    printf("chyba otevreni souboru\n");
	    return 1;
	}
  printf("Otevren soubor\n" );

  spojovy_seznam s;
	s.zacatek = NULL;
	s.konec = NULL;

  printf("Vytvoren seznam\n" );

  nacist_soubor(&s, soubor);

  printf("nacten seznam\n" );

  vypis_seznam(&s);

  printf("vypsan seznam\n" );

  fclose(soubor);
	zrus_seznam(&s);
	return 0;
}
