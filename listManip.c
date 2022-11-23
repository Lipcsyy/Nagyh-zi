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

bool listaTartalmaz(listaElem* eleje, char* nev){

    listaElem head = eleje;

    while (head != NULL)
    {
        if (strcmp(head->kiadas->nev,nev) == 0)
        {
            return true;
        }
        head = head->kov;
    }

    return false;
}