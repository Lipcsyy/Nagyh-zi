#include <stdio.h>

#include "kiadas.h"
#include "szamla.h"
#include "print.h"


/// @brisf Bevétel megadása, ez a függvény fogja hozzáadni a pénzt a számládhoz.
void bevetel(){

    headerPrint("BEVETEL MEGADASA");

    int bev = 0;
    printf("Add meg, hogy mennyi penzt szeretnel feltolteni a szamlara!\n");
    scanf("%d", &bev);

    printf("\033[A\33[2K\033[A\33[2K");

    szamla += bev;
    szamlaWriter(szamla);
    printf("Az uj egyenleg: %d \n", szamla);

    footerPrint("BEVETEL MEGADASA");
}