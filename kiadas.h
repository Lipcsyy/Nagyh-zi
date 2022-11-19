#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum Kategoria
{
    vendeglatas,
    szepsegapolas,
    ruhazat,
    szorakozas,
    bevasarlas,
    egyeb

} Kategoria;

typedef struct Kiadas
{
    int osszeg;
    char nev[50];
    Kategoria kategoria;
    struct tm datum;
    int id;

} Kiadas;

typedef struct listaElem{
    
    Kiadas* kiadas;
    struct listaElem* kov;

}  listaElem;

/// @brief Ez a függvény kezeli a kiadásokat, itt tudod megadni, milyen tételt és mennyiért vásároltál, illetve, hogy milyen kategóriába tartozik
/// @param t Ezt szükséges megadni a függvénynek, hogy tudhassa az aktuális időt, ami szerint válogatja szét külön fileokba a termékeket
void kiadas(time_t t);

/// @brief Ez a fuggveny beleirja a szamlaba a megadott osszeget
/// @param osszeg Itt add meg, hogy mennyit szeretnel hozzadni a szamlahoz.
void szamlaWriter(int osszeg);

/// @brief Ez a függvény leellenőrzi, hogy a megnyitni kívánt file létezik-e?
/// @param filename Paraméterként a megnyitni kívánt file nevét kell megadni
/// @return Visszadja, hogy létezik-e a file.
bool fileExists(char *filename);

/// @brief Ez a függvény írja bele az aktuális hónap file-ba az adatokat
/// @param fileName A filenév, ami alapján checkoljuk, hogy hova kell írni
/// @param kiadasok Az adatok, amiket bele szeretnénk írni a fileba
/// @param length A hossza az kiadasok tombnek
void kiadasWriter(char* fileName, listaElem* eleje, const char* mode);

/// @brief Ez a fuggveny a lancolt lista vegere fuzi a kovetkezo elemet
/// @param eleje itt add meg a láncolt lista elejere mutato pointert
/// @param kiadas itt meg add meg az adott elemet, amit hozzá szeretnél fűzni a listához
void listaVegFuz(listaElem** eleje, Kiadas* kiadas);

/// @brief Ez a fuggveny a lancolt lista felszabadítja
/// @param eleje A lancolt lista elejere mutato pointer
void listaFelszabadit(listaElem* eleje);

void kiadasEdit();

/// @brief ez a függvény kilistazza az kert hónap adatait
void kiadasList();
