#ifndef LISTAMANIP_H
#define LISTAMANIP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "header.h"

/// @brief Ez a fuggveny a láncolt lista vegehez fuz egy elemet
/// @param eleje A láncolt lista elejere mutato pointer
/// @param kiadas Az adott kiadas, amit a vegehez szeretnel fuzni
void listaVegFuz(listaElem** eleje, Kiadas* kiadas);

/// @brief Ez a fuggveny felszabadítja a láncolt lista által lefoglalt helyet
/// @param eleje A láncolt lista elejere mutato pointer
void listaFelszabadit(listaElem* eleje);

/// @brief Ez a fuggveny megmondja, hogy egy lancolt lista tartalmaz-e egy elemet
/// @param eleje A láncolt lista elejere mutato pointer
/// @param nev A kivant kiadas neve
/// @return Igaz, ha tartalmazza, hamis ha nem
bool listaTartalmaz(countOfElem* eleje, char* nev);

/// @brief Ez a lista a kiadasok számlálásánál fűzi a végéhez az adott tételt
/// @param eleje A láncolt lista elejere mutato pointer
/// @param nev Az amit a vegere szeretnenk fuzni
void statListaVegFuz(countOfElem** eleje, char* nev);

/// @brief Ez a fuggveny felszabadítja a láncolt lista által lefoglalt helyet 
/// @param eleje 
void statListaFelszabadit(countOfElem* eleje);

#endif