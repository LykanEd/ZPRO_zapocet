#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

//--------------------------------------------------------------
// DEKLARACE STRUKTUR

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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
// PRACE SE SKTRUKTURAMI

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

uzel* vytvor_uzel(data_typ* data){
  vypis_data(data);
	uzel* uzl = malloc(sizeof(uzel));
	if(uzl == NULL){
		printf("chyba alokace\n");
		return NULL;
	}
  printf("vytvor_uzel succsesful\n");

	uzl->data = data;
  printf("data prirazena uzlu\n");
	uzl->naslednik = NULL;
  printf("naslednik nastaven NULL \n" );
  vypis_data(uzl->data);
  printf("data vypsana\n " );

	return uzl;
}

void na_zacatek(spojovy_seznam* s, data_typ* data){
  printf("Davam na na_zacatek\n");
  vypis_data(data);
	uzel* uzl = vytvor_uzel(data);
	if (uzl == NULL){
		free(data);
	}
  printf("vytvoren uzel\n" );
  vypis_data(uzl->data);

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
	if(naslednik == NULL)
		printf("Prazdny seznam.\n");
	while(naslednik != NULL){
		vypis_data(naslednik->data);
		naslednik = naslednik->naslednik;
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
// ZPRACOVANI SOUBORU

void odstran_mezery(char string[]){
  int j = 0;
  int i = 0;
  // odstraneni mezer na zacatku
  while (string[i] == ' ') {
    i++;
  }
  for (i; i < strlen(string); i++) {
    if (string[i] != ' ') {
      string[j] = string[i];
      j++;
    } else{
      for (int k = i; k < strlen(string); k++) {
        if (string[k] != ' ') {
          string[j] = ' ';
          j++;
          i = k-1;
          break;
        }
      }
    }
  }
  string[j] = '\0';
  printf(">>%s<<\n", string);
}

void nacti_polozku(const char* string, int zacatek, char polozka[]){
  int vytvoreno = 0;
  // pokud by byly stredniky primo u sebe nebo za strednikem
  // konec radku
  if (string[zacatek] == ';' || string[zacatek] == '\0') {
    polozka[0] = '\0';
  }else {
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
  }
}

int prazdna_polozka(char* polozka){
	for (int i = 0; i < strlen(polozka); i++){
		if (isspace(polozka[i])){
			return 1;
		}
	}
	return 0;
}

void zpracuj_radek(const char* string, data_typ* data){
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
    data->nazev[0] = '\0';
    printf("Prazdny radek\n" );
  } else{
    int zacatek = 0;
    nacti_polozku(string, zacatek, data->nazev);
    zacatek += strlen(data->nazev)+1;

    nacti_polozku(string, zacatek, data->typ);
    zacatek += strlen(data->typ)+1;

    nacti_polozku(string, zacatek, data->cislo);
    zacatek += strlen(data->cislo)+1;

    nacti_polozku(string, zacatek, data->odpovedny);
    zacatek += strlen(data->odpovedny)+1;

    nacti_polozku(string, zacatek, data->datum);
    zacatek += strlen(data->datum)+1;

    nacti_polozku(string, zacatek, data->kontrola);
    zacatek += strlen(data->kontrola)+1;

    nacti_polozku(string, zacatek, data->stav);

    vypis_data(data);
  }

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
    int prazdne = 0;

    prazdne += TEST_prazdny_string(data->nazev);
    prazdne += TEST_prazdny_string(data->typ);
    prazdne += TEST_prazdny_string(data->cislo);
    prazdne += TEST_prazdny_string(data->odpovedny);
    return prazdne;
}

int TEST_datum(data_typ* data){
  char *ptr;
  printf("%s\n", data->datum);
  char* date = data->datum;
  printf("%s\n", date);

  int den = strtol(date, &ptr, 10);
  int mesic = strtol(ptr+1, &ptr, 10);
  int rok = strtol(ptr+1, &ptr, 10);
  printf("%d.%d.%d\n", den,mesic,rok);

  if (1900 > rok || 2020 < rok) {
    printf("chyba roku\n");
    return 1;
  }
  if (1 > mesic || 12 < mesic) {
    printf("chyba mesice\n");
    return 1;
  }
  if (1 > den || 31 < den) {
    printf("chyba dne\n");
    return 1;
  }
  printf("datum OK");
  return 0;
}

//--------------------------------------------------------------

int nacist_soubor(spojovy_seznam* s, FILE* soubor){
	while(!feof(soubor)){
    printf("NOVY radek\n");
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
			break;
		}
    printf("data vytvorena, zpracovavam radek\n");
		// zpracovani radku
		zpracuj_radek(radek, data);
    if (data->nazev[0] == '\0') {
      if (!feof(soubor)) {
        // nebude zpusteno kvuli prazdnemu poslednimu radku
        printf("Chybny format souboru\n");
        zrus_seznam(s);
        return 1;
      }
    } else{
      // testovani dat
      int pocet_udaju = TEST_pocet_udaju(data);
      if (pocet_udaju != 4) {
        printf("Testovaci error - chybny pocet udaju.\n");
        zrus_seznam(s);
        return 1;
      }
      if (TEST_datum(data) != 0) {
        printf("Testovaci error - chybny format data.\n");
        zrus_seznam(s);
        return 1;
      }
      odstran_mezery(data->cislo);
      printf(">%s<\n", data->cislo);

      printf("radek zpracovan\n");

      //vypis_data(data);
      printf("\n" );

  		//vlozit data do seznamu
  		na_zacatek(s, data);
      printf("radek vlozen do seznamu\n");
      }
	}
  return 0;
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

  int check = nacist_soubor(&s, soubor);
  if (check != 0) {
    printf("Nekompletni seznam\n");
    return 0;
  }

  printf("nacten seznam\n" );
  if (s.zacatek != NULL) {
    vypis_data(s.zacatek->data);
  } else{
    printf("Prazdny seznam");
  }
  vypis_seznam(&s);

  printf("vypsan seznam\n" );

  fclose(soubor);
	zrus_seznam(&s);
	return 0;
}
