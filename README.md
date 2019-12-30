# Započtový úkol na ZPRO

Po spuštění programu je potřeba zadat do příkazové řádky název souboru, který se má zpracovat (včetně přípony souboru). 
Seřazený soubor je uložen do stejné složky pod názvem SERAZENO_/nazev puvodniho souboru/./pripona puvodniho souboru/.
V příkazovém řádku proběhne výpis položek, které nemají v pořádku kontrolu (tj. v políčku stav není "OK").

## Testovací soubory

### test0.txt
Prázdný soubor.

Program vypíše, že soubor s daty je prázdný, a nic neuloží.

### test1.txt
Chybný počet údajů.

Program vypíše, že na daném řádku je chybný počet údajů, a nic neuloží.

### test2.txt
Chybějící údaje o kontrole.

Program vypíše položky s nevyhovující kontrolou, sežazený seznam uloží.

### test5.txt
Špatný formát data.

Program vypíše, že na daném řádku je chybný formát data, a nic neuloží.

### test7.txt
Špatný formát inventárního čísla.

Program vypíše, že na daném řádku je chybný formát inventárního čísla, a nic neuloží.

### test9.txt
Všechny položky v pořádku.

Program vypíše položky s nevyhovující kontrolou, sežazený seznam uloží.

## Test správnosti řádku a jednotlivých položek
1. Na řádku musí být právě 6 středníků.
2. Položka název, typ, inventarní číslo a zodpovědná osoba nesmí být prázdné.
3. Položka datum kontroly, kontrolujici osoba a stav musí být buď všechny prázdné nebo všechny neprázdné.
4. Den kontroly musí být v rozmezí 1-31, měsíc kontroly 1-12, rok kontroly 1900-2020.
5. Inventární číslo musí být v uvedeném tvaru, tedy: A7-12345679/111 (kontroluje se délka, pozice číslic a písmen, pozice lomítka a pomlčky)
