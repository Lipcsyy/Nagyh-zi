#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "debugmalloc.h"
#include "kiadas.h"
#include "bevetel.h"
#include "szamla.h"
#include "print.h"
#include "statistics.h"

//----------------------------------------------------------Elso inditas-------------------------------------------------------------

#define firstSzamla 200000

int main(void)
{   
    
    time_t t = time(NULL);  //idő megadasa a fuggvenyeknek, mindig kell az aktualis futashoz
    int input; 
    
    //Szeretnénk megnézni, hogy valaha futott-e már a program, ha igen, akkor a számlán lévő összeg változott, ezt be szeretnénk olvasni az eltárolt "számla.txt" fileból

    if (fileExists("szamla.txt"))
    {
        FILE* fp = fopen("szamla.txt","r"); 
        int _szamla;
        fscanf(fp, "%d", &_szamla);
        fclose(fp);
        szamla = _szamla;
    }
    else
    {   
        szamlaWriter(firstSzamla);
        szamla = firstSzamla;
    }

    menuPrint();

while (scanf(" %d", &input) == 1 && input != 6)
    {
         switch (input)
         {
            case 1:
                kiadas(t);
                break;
            case 2:
                bevetel();
                break;
            case 3:
                kiadasList();
                break;
            case 4:
                kiadasEdit();
                break;
            case 5:
                statistics();
                break;

            default:
                break;
         }

         menuPrint();
    }

    return 0;
}


