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

        printf("%s\n", head->kiadas->nev);
        free(head->kiadas);
        printf("head utan\n");
        free(head);

        head = temp;
    }
    
}