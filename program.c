#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "header.h"
#include <dirent.h> 

//----------------------------------------------------------Elso inditas-------------------------------------------------------------

#define firstSzamla 200000

//------------------------------------------------------------Program----------------------------------------------------------------

int szamla = 0;
char* tags[] = {"vendeglatas","szepsegapolas","ruhazat", "szorakozas", "bevasarlas", "egyeb" };

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

//---------------------------------------------Függvények, amiket máshova kell tegyek majd-------------------------------------

void kiadasWriter(char* fileName, listaElem* eleje);
void listaVegFuz(listaElem** eleje, time_t t, Kiadas* kiadas);

int main(void)
{   
    
    time_t t = time(NULL);  //idő megadasa a fuggvenyeknek, mindig kell az aktualis futashoz
    int input; 
    
    //Szeretnénk megnézni, hogy valaha futott-e már a program, ha igen, akkor a számlán lévő összeg változott, ezt be szeretnénk olvasni az eltárolt "számla.txt" fileból

    if (fileExits("szamla.txt"))
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

    kiadasList();

    // menuPrint();

    // while (scanf("%d", &input) == 1 && input != 6)
    // {
    //     switch (input)
    //     {
    //     case 1:
    //         kiadas(&szamla);
    //         break;
    //     case 2:
    //         bevetel();
    //         break;
    //     case 3:
    //         kiadasList();
    //         break;
    //     case 4:
    //         kiadasEdit();
    //         break;
    //     case 5:
    //         statistics();
    //         break;

    //     default:
    //         break;
    //     }

    //     menuPrint();
    // }

    return 0;
}

//----------------------------------------------------------Függvények-------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------


/// @brief Ez a függvény kezeli a kiadásokat, itt tudod megadni, milyen tételt és mennyiért vásároltál, illetve, hogy milyen kategóriába tartozik
/// @param t Ezt szükséges megadni a függvénynek, hogy tudhassa az aktuális időt, ami szerint válogatja szét külön fileokba a termékeket
void kiadas(time_t t)
{

    headerPrint("KIADAS BEVITELE");
    
    Kiadas* kiadasok = (Kiadas*) malloc(1*sizeof(Kiadas)); //több tétel beviteléhez kell -- 1-nél valamiért rosal asz

    char input;
    bool canContinue = true;

    listaElem* eleje = NULL; //láncolt lista eleje

    int kiadasokCount = 0;
    int kiadasokHossz = 1;

    while (canContinue)
    {
        Kiadas* kiadasPt = (Kiadas*) malloc(sizeof(Kiadas));
        kiadasPt->datum = *localtime(&t);

        kiadasPt->datum.tm_year += 1900;
        kiadasPt->datum.tm_mon += 1;
        kiadasPt->id = kiadasPt->datum.tm_sec + kiadasPt->datum.tm_min*60 + kiadasPt->datum.tm_hour*60*60 + kiadasPt->datum.tm_mday*24*60*60 + kiadasokCount;

        printf("Add meg a tetel nevet!\n");
        scanf("%s", kiadasPt->nev);
        printf("\033[A\33[2K\033[A\33[2K");

        printf("Add meg a tetel arat!\n");
        scanf("%d",&(kiadasPt->osszeg));

        printf("\033[A\33[2K\033[A\33[2K");

        printf("Add meg a kategoriajat!\n");

        while (scanf("%d", &(kiadasPt->kategoria)) == 1 && (kiadasPt->kategoria > 6 || kiadasPt->kategoria < 0 ))
        {
            printf("Add meg a kategoriajat!\n");
            printf("\033[A\33[2K\033[A\33[2K");
        }   

        printf("\033[A\33[2K\033[A\33[2K");

        //--------------------------------------------láncolt listával

        listaVegFuz(&eleje, t, kiadasPt);

        //-----------------------------------------------malloccal

        // if(kiadasokCount >= kiadasokHossz)
        // {   
        //     kiadasokHossz *= 2;    
        //     //kiadasok = (Kiadas*) realloc(kiadasok, sizeof(Kiadas)*(kiadasokHossz));
        // }

        //kiadasok[kiadasokCount] = kiadas;
        //kiadasokCount++;
        
        printf("Szeretned folytatni? (I) Igen (N) Nem : ");
        canContinue = scanf(" %c", &input) == 1 && (input == 'i' || input == 'I');
        
    }

    //összeszámoljuk, mennyit költene összesen most a felhasználó

    int kiadasSum = 0;
    listaElem* mozgo = eleje;
    printf("%p\n", mozgo);

    while (mozgo != NULL) 
    {
        kiadasSum+= mozgo->kiadas->osszeg;
        mozgo = mozgo->kov;
    }

   
    //ha nincsen eleg penze, akkor hibaüzenettel visszatérünk, ha pedig sikeres, akkor végrehajtjuk a tranzakciot

    if (kiadasSum > szamla)
    {
        printf("Sikertelen bevitel nincs %d Ft a szamladon, a hianyzo osszeg : %d Ft \n", kiadasSum, kiadasSum-szamla);
    }
    else
    {   

        mozgo = eleje;
        printf("%p\n", mozgo);


        while (mozgo != NULL)
        {
            printf("Sikeres bevitel: %s - %d HUF\n", mozgo->kiadas->nev, mozgo->kiadas->osszeg);
            mozgo = mozgo->kov;
        }

        szamla -= kiadasSum; //a szamlabol kivonom az osszeget
        szamlaWriter(szamla); //eltarolom az uj egyenleget

        //itt alakítom ki a filenak a nevét

        char fYear[5];
        sprintf(fYear,"%d", eleje->kiadas->datum.tm_year);

        char fMonth[3];
        sprintf(fMonth,"%d",eleje->kiadas->datum.tm_mon);

        char fileName[11];
        strcpy(fileName, fYear);
        strcat(fileName,"_");
        strcat(fileName,fMonth);

        kiadasWriter(fileName, eleje);
        free(kiadasok);
    }
    
}


/// @brief Bevétel megadása, ez a függvény fogja hozzáadni a pénzt a számládhoz.
void bevetel(){

    headerPrint("BEVETEL MEGADASA");

    int bev = 0;
    printf("Add meg, hogy mennyi penzt szeretnel feltolteni a szamlara!\n");
    scanf("%d", &bev);

    printf("\033[A\33[2K\033[A\33[2K");

    szamla += bev;
    szamlaWriter(szamla);
    printf("Az uj egyenleg: %d \n", szamla);
}

/// @brief ez a függvény kilistazza az kert hónap adatait
void kiadasList(){

    headerPrint("KIADAS LISTAZASA");

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
            if (sscanf(dir->d_name,"%d_%d") != 0)
            {
                printf("SPECIAL: %s", dir->d_name);
            }

        }
        closedir(d);
    }

}


void kiadasEdit(){
    printf("KiadasEdit\n");
}


void statistics(){
    printf("Statistics\n");
}


void menuPrint(){
    printf("Szamlan levo osszeg: %d HUF \n", szamla);

    printf("(1) - Uj kiadas bevitele\n");
    printf("(2) - Uj bevetel bevitele\n");
    printf("(3) - Kiadasok listazasa \n");
    printf("(4) - Kiadasok szerkesztese\n");
    printf("(5) - Statisztikak\n");
    printf("(6) - Kilepes\n");
}


///@brief Ezzel függvénnyel tudjuk az egyes részek headerjét
///@param header Itt tudod megadni mi legyen a header-be írva
void headerPrint(char* header)
{

    printf("\n");

    for (int i = 0; i < 30; i++)
    {
        printf("-");
    }

    printf("%s", header);

    for (int i = 0; i < 30; i++)
    {
        printf("-");
    }

    printf("\n\n");

    // for (int i = 0; i < 8; i++)
    // {
    //     printf("\033[A\33[2K");
    // }
}


/// @brief Ez a fuggveny beleirja a szamlaba a megadott osszeget
/// @param osszeg Itt add meg, hogy mennyit szeretnel hozzadni a szamlahoz.
void szamlaWriter(int osszeg){
    FILE* fp;

    fp = fopen("szamla.txt","w");

    fprintf(fp,"%d", osszeg);

    fclose(fp);
}


/// @brief Ez a függvény leellenőrzi, hogy a megnyitni kívánt file létezik-e?
/// @param filename Paraméterként a megnyitni kívánt file nevét kell megadni
/// @return Visszadja, hogy létezik-e a file.
bool fileExits(char *filename)
{
    FILE* fp;
    fp = fopen(filename,"r");
    if (fp == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/// @brief Ez a függvény írja bele az aktuális hónap file-ba az adatokat
/// @param fileName A filenév, ami alapján checkoljuk, hogy hova kell írni
/// @param kiadasok Az adatok, amiket bele szeretnénk írni a fileba
/// @param length A hossza az kiadasok tombnek
void kiadasWriter(char* fileName, listaElem* eleje)
{

    listaElem* head = eleje;

    if (fileExits(fileName)) //ha létezik akkor csak appendelek a végére
    {
        FILE* fp = fopen(fileName, "a");
        while (head != NULL)
        { 
            fprintf(fp,"%s_%d_%s_%d\n", head->kiadas->nev, head->kiadas->osszeg,tags[head->kiadas->kategoria], head->kiadas->id);  
            head = head->kov;
        }
            
        fclose(fp);
    }
    else //ha nem akkor pedig letrehozom
    {
        FILE* fp = fopen(fileName, "w");

        while (head != NULL)
        {
            fprintf(fp,"%s_%d_%s_%d\n", head->kiadas->nev, head->kiadas->osszeg,tags[head->kiadas->kategoria], head->kiadas->id);  
            head = head->kov;
        }
        
        fclose(fp);

    }
}


/// @brief Ez a fuggveny a lancolt lista vegere fuzi a kovetkezo elemet
/// @param eleje 
void listaVegFuz(listaElem** eleje, time_t t, Kiadas* kiadas)
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
