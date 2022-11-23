#ifndef HEADER_H
#define HEADER_H

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

typedef struct countOfElem
{
    int count;
    char nev[50];
    struct countOfElem* kov;
} countOfElem;

#endif 

