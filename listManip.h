#ifndef LISTAMANIP_H
#define LISTAMANIP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "header.h"


void listaVegFuz(listaElem** eleje, Kiadas* kiadas);

void listaFelszabadit(listaElem* eleje);

bool listaTartalmaz(countOfElem* eleje, char* nev);

void statListaVegFuz(countOfElem** eleje, char* nev);

void statListaFelszabadit(countOfElem* eleje);

#endif