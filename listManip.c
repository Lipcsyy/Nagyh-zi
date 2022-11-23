#include "listManip.h"

void listaVegFuz(listaElem** eleje, Kiadas* kiadas)
{
    
    listaElem* uj;

    uj = (listaElem*) malloc(sizeof(listaElem));

    uj->kiadas = kiadas;
    uj->kov = NULL;
    
    if (*eleje == NULL)
    {    
        *eleje = uj;
    }
    else
    {   
        listaElem* mozgo = *eleje;

        while (mozgo->kov != NULL) 
        {   
            mozgo = mozgo->kov;
        }
        mozgo->kov = uj; 
    }  
}


void listaFelszabadit(listaElem* eleje)
{   
    listaElem* head = eleje;

    while (head != NULL)
    {
        listaElem* temp = head->kov;

        free(head->kiadas);
        free(head);

        head = temp;
    }
}


bool listaTartalmaz(countOfElem* eleje, char* nev){

    countOfElem* head = eleje;

    while (head != NULL)
    {
        if (strcmp(head->nev,nev) == 0)
        {
            head->count += 1; // itt noveljuk
            return true;
        }
        head = head->kov;
    }

    return false;
}


void statListaVegFuz(countOfElem** eleje, char* nev)
{
    countOfElem* uj;

    uj = (countOfElem*) malloc(sizeof(countOfElem));

    strcpy(uj->nev, nev);
    uj->count = 1;
    uj->kov = NULL;
    
    if (*eleje == NULL)
    {    
        *eleje = uj;
    }
    else
    {   
        countOfElem* mozgo = *eleje;

        while (mozgo->kov != NULL) 
        {   
            mozgo = mozgo->kov;
        }
        mozgo->kov = uj; 
    }  
}

void statListaFelszabadit(countOfElem* eleje)
{
    countOfElem* head = eleje;

    while (head != NULL)
    {
        countOfElem* temp = head->kov;

        free(head);

        head = temp;
    }
}