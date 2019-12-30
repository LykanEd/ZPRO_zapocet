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

// funkce pro nacteni jednoho radku textu z terminalu
bool getLine(char* string, int maxDelka)
{
    for (int i = 0; i < maxDelka; i++) {
        // precti znak z terminalu
        int znak = getchar();

        // zkontroluj, jestli neni konec vstupu
        if (znak != EOF && znak != '\n')
            string[i] = znak;
        else {
            // konec vstupu - ukonci string a prerus cyklus
            string[i] = '\0';
            return true;
        }
    }

    // v cyklu jsme nedosli ke znaku EOF ani k '\n' - vstup je delsi nez (maxDelka - 1)
    // (vypisujeme maxDelka - 1, protoze posledni znak musi byt '\0', ktery ale uzivatel nezadava)
    printf("Chyba: vstup je delsi nez maximalni povolena delka %d znaku.\n", maxDelka - 1);
    return false;
}


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
  printf("Nazev polozky ; Typ polozky ; Inventarni cislo ; Odpovedna osoba ; Datum kontroly ; Kontrolujici osoba ; Stav polozky\n");
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

void uloz_seznam(char* puvodni_soubor, spojovy_seznam* s){
  char novy_soubor[35] = "ZPRACOVANO_";
  strcat(novy_soubor, puvodni_soubor);

  uzel* naslednik = s->zacatek;
  if(naslednik == NULL){
    printf("Prazdny seznam.\n");
    return;
  }
  FILE* zapis_soubor = fopen(novy_soubor, "w");
	if (zapis_soubor == NULL) {
	    printf("chyba otevreni souboru\n");
	    return;
	}

  fprintf(zapis_soubor, "%s\n", "# Nazev polozky   ; Typ polozky ; Inventarni cislo ; Odpovedna osoba ; Datum kontroly ; Kontrolujici osoba ; Stav polozky");

  while(naslednik != NULL){
    fprintf(zapis_soubor, "%s ;  %s ;  %s ;  %s ;  %s ;  %s ;  %s\n",
      naslednik->data->nazev,
      naslednik->data->typ,
      naslednik->data->cislo,
      naslednik->data->odpovedny,
      naslednik->data->datum,
      naslednik->data->kontrola,
      naslednik->data->stav
    );
    naslednik = naslednik->naslednik;
  }
  printf("Zapsano do souboru %s\n", novy_soubor);

  fclose(zapis_soubor);

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
  if (pocet_stredniku != 6 && pocet_stredniku != 3) {
    data->nazev[0] = '\0';
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
  }
}

void spatna_kontrola(data_typ* data, spojovy_seznam* bez_kontroly) {
  odstran_mezery(data->stav);
  //printf(">>>>%s\n", data->stav);

  if (strcmp(data->stav, "OK") != 0) {
    printf("Stav neni OK, pridano do seznamu chybici kontroly.\n");
    na_zacatek(bez_kontroly, data);
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
  /*
  zkontroluje pocet udaju na radku, pokud jsou vsechny polozky vyplnene nebo
  chybi vsechny polozky tykajici se kontroly, vrati hodnotu 4, jinak vraci jine
  hodnoty int
  */
    int prazdne = 0;

    prazdne += TEST_prazdny_string(data->nazev);
    prazdne += TEST_prazdny_string(data->typ);
    prazdne += TEST_prazdny_string(data->cislo);
    prazdne += TEST_prazdny_string(data->odpovedny);

    int kontrola_stavu = 0;
    kontrola_stavu += TEST_prazdny_string(data->datum);
    kontrola_stavu += TEST_prazdny_string(data->kontrola);
    kontrola_stavu += TEST_prazdny_string(data->stav);

    /*
    pokud jsou vyplnene jen nejake polozky tykajici se kontroly,
    zmeni navratovou hodnotu tak, aby byl chybny pocit udaju
    */
    if (kontrola_stavu != 3 && kontrola_stavu != 0) {
      prazdne--;
    }

    return prazdne;
}

int TEST_datum(data_typ* data){
  if (TEST_prazdny_string(data->datum) == 0) {
    return 0;
  }
  char *ptr;
  char* date = data->datum;

  int den = strtol(date, &ptr, 10);
  int mesic = strtol(ptr+1, &ptr, 10);
  int rok = strtol(ptr+1, &ptr, 10);

  if (1900 > rok || 2020 < rok) {
    printf("Chyba roku\n");
    return 1;
  }
  if (1 > mesic || 12 < mesic) {
    printf("Chyba mesice\n");
    return 1;
  }
  if (1 > den || 31 < den) {
    printf("Chyba dne\n");
    return 1;
  }
  return 0;
}

int TEST_inventarni(data_typ* data){
  odstran_mezery(data->cislo);

  if (strlen(data->cislo)==15) {
    int check = 0;
    int pozice = 0;

    // kontrola prvni je velke pismeno
    if (data->cislo[pozice] >= 'A' && data->cislo[pozice] <= 'Z') {
      pozice++;
    } else{
      return 1;
    }
    // kontrola druhe je cislo
    if (data->cislo[pozice] >= '0' && data->cislo[pozice] <= '9') {
      pozice++;
    } else{
      return 1;
    }
    // kontrola treti je pomlcka
    if (data->cislo[pozice] == '-') {
      pozice++;
    } else{
      return 1;
    }
    // kontrola dalsich osm je cislo
    for (pozice; pozice < 11; pozice++) {
      if (data->cislo[pozice] >= '0' && data->cislo[pozice] <= '9') {
      } else{
        return 1;
      }
    }
    // kontrola dvanacte je lomitko
    if (data->cislo[pozice] == '/') {
      pozice++;
    } else{
      return 1;
    }
    // kontrolaposledni tri jsou cisla
    for (pozice; pozice < 15; pozice++) {
      if (data->cislo[pozice] >= '0' && data->cislo[pozice] <= '9') {
      } else{
        return 1;
      }
    }

  } else{
    return 1;
  }

  return 0;
}

//--------------------------------------------------------------
// serazeni inventranich cisel

uzel* max(uzel* zacatek){
	uzel* maximum = zacatek;
	while(zacatek != NULL){
    int srovnani = strcmp(maximum->data->cislo, zacatek->data->cislo);
		if(srovnani > 0)
			maximum = zacatek;
    zacatek = zacatek->naslednik;
	}
	return maximum;
}

void serad_seznam(spojovy_seznam* s){
  uzel* dalsi = s->zacatek;
  while(dalsi != NULL){
    uzel* maximum = max(dalsi);
    data_typ* tmp = dalsi->data;
    dalsi->data = maximum->data;
    maximum->data = tmp;
    dalsi = dalsi->naslednik;
  }
}

//--------------------------------------------------------------

int nacist_soubor(spojovy_seznam* s, FILE* soubor, spojovy_seznam* spatny_stav){
  int cislo_radku = 1;
  while(!feof(soubor)){
    printf("NOVY radek - cislo %d\n", cislo_radku);
    cislo_radku++;
		// Precteni radku
    printf("Nacten radek\n");
		char radek[1000];
		bool nacteno = fgetLine(soubor, radek, 1000);
		// preskocit hlavicku
		if (radek[0] == '#'){
      printf("Preskocen radek s komentarem\n\n");
			continue;
      }
		// vytvoreni dat
    printf("Radek neni komentar, vytvarim data\n");
		data_typ* data = malloc(sizeof(data_typ));
		if(data == NULL){
			printf("chyba alokace\n");
			fclose(soubor);
			break;
		}
    printf("Data vytvorena, zpracovavam radek\n");
		// zpracovani radku
		zpracuj_radek(radek, data);
    if (data->nazev[0] == '\0') {
      if (!feof(soubor)) {
        // nebude zpusteno kvuli prazdnemu poslednimu radku
        printf("Chybny nazev souboru\n");
        return 1;
      }
    } else{
      // testovani dat
      int pocet_udaju = TEST_pocet_udaju(data);
      if (pocet_udaju != 4) {
        printf("Testovaci error - chybny pocet udaju.\n");
        return 1;
      }
      if (TEST_datum(data) != 0) {
        printf("Testovaci error - chybny format data.\n");
        return 1;
      }
      if (TEST_inventarni(data) != 0) {
        printf("Testovaci error - chybny format inventarniho cisla.\n");
        return 1;
      }

      spatna_kontrola(data, spatny_stav);

      printf("radek zpracovan\n");

  		//vlozit data do seznamu
  		na_zacatek(s, data);
      printf("radek vlozen do seznamu\n\n");
      }
	}
  return 0;
}

void uzavri(FILE* soubor, spojovy_seznam* s, spojovy_seznam* spatny_stav) {
  zrus_seznam(s);
  zrus_seznam(spatny_stav);
  fclose(soubor);
  printf("Soubor uzavren, seznamy smazany.\n");
}



int main(){

  printf("Zadejte nazev souboru s daty: ");
  int max_delka = 40;
  char soubor_nazev[max_delka];
  getLine(soubor_nazev, max_delka);
  printf("\n");


  FILE* soubor = fopen(soubor_nazev, "r");
	if (soubor == NULL) {
	    printf("chyba otevreni souboru\n");
	    return 1;
	}
  printf("Otevren soubor\n" );

  spojovy_seznam s;
	s.zacatek = NULL;
	s.konec = NULL;

  spojovy_seznam spatny_stav;
	spatny_stav.zacatek = NULL;
	spatny_stav.konec = NULL;

  printf("Vytvoren seznam\n" );

  int check = nacist_soubor(&s, soubor, &spatny_stav);
  if (check == 2) {
    uzavri(soubor, &s, &spatny_stav);
    return 0;
  }
  if (check != 0) {
    printf("Nekompletni seznam\n");
    uzavri(soubor, &s, &spatny_stav);
    return 0;
  }

  printf("Radim seznam\n");
  serad_seznam(&s);
  printf("Seznam serazen\n");

  printf("Nacten seznam\n" );
  if (s.zacatek != NULL) {
    // vypis_seznam(&s);
    // printf("Seznam vypsan\n" );

    printf("Ukladam do souboru.\n");
    uloz_seznam(soubor_nazev, &s);
    printf("Ulozeno do souboru.\n");

    printf("Vypisuji seznam polozek s nevyhovujicim stavem:\n");
    vypis_seznam(&spatny_stav);
    printf("Seznam vypsan\n");
  } else{
    printf("\nSeznam neobsahuje zadna data\n");
  }

  uzavri(soubor, &s, &spatny_stav);

	return 0;
}
