#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>



void odstran_mezery(char string[,]){
  int j = 0;
  char nove[strlen(string)];
  int i = 0;
  while (string[i] == ' ') {
    i++;
  }
  for (i; i < strlen(string); i++) {
    if (string[i] != ' ') {
      nove[j] = string[i];
      j++;
    } else{
      for (int k = i; k < strlen(string); k++) {
        if (string[k] != ' ') {
          nove[j] = ' ';
          j++;
          i = k-1;
          break;
        }
      }
    }
  }

  nove[j] = '\0';
  for (int i = 0; i < strlen(nove); i++) {
    string[i] = nove[i];
  }
  printf(">>%s<<\n", string);
}

void serad_inventarni_cislo(char icislo[]) {
  odstran_mezery(icislo);
  printf(">%s<\n", icislo);

}

void testovani_data(){
  char str[30] = "10.3.2000";
  char *ptr;
  int ret;

  int den = strtol(str, &ptr, 10);
  printf("%d\n", den);
  int mesic = strtol(ptr+1, &ptr, 10);
  printf("%d\n", mesic);
  int rok = strtol(ptr, &ptr, 10);
  printf("%d.%d.%def\n", den,mesic,rok);

  ret = strtol(str, &ptr, 10);
  printf("The number(unsigned long integer) is %ld\n", ret);
  printf("String part is |%s|", ptr);
}

void testing_mezery() {
  // char* neco = "123456";
  // for (size_t i = 0; i < strlen(neco); i++) {
  //   char dalsi = neco[i+1];
  //   printf("%c\n", dalsi);
  //   printf("%c\n", neco[i]);
  // }
  // printf("%s\n", neco);
  char neco[] = "   ahoj    jak  ty   ";
  // odstran_mezery("ahoj");
  // odstran_mezery(" ahoj");
  // odstran_mezery("ahoj ");
  // odstran_mezery("  ahoj  ");
  // odstran_mezery("  ahoj jak  ");
  odstran_mezery(neco);
  printf(">%s<\n", neco);
}

 int main() {
  char* a = 'a';
  char* b = 'b';
  a = b;
  printf("%c\n", a);
  printf("%c\n", b);

  char* c = "ahojky ";
  odstran_mezery(c);
  printf(">%s<\n", c);

  return 0;
}
