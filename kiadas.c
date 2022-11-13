#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

#include "szamla.h"
#include "kiadas.h"
#include "print.h"


char* tags[] = {"vendeglatas","szepsegapolas","ruhazat", "szorakozas", "bevasarlas", "egyeb" };

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
        kiadasPt->id = kiadasPt->datum.tm_mday*1000000 + kiadasPt->datum.tm_hour*10000 + kiadasPt->datum.tm_min*100 + kiadasPt->datum.tm_sec + kiadasokCount; 

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
        kiadasokCount++;
        
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


void szamlaWriter(int osszeg){
    FILE* fp;

    fp = fopen("szamla.txt","w");

    fprintf(fp,"%d", szamla);

    fclose(fp);
}


bool fileExists(char *filename)
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


void kiadasWriter(char* fileName, listaElem* eleje)
{

    listaElem* head = eleje;

    if (fileExists(fileName)) //ha létezik akkor csak appendelek a végére
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


void kiadasEdit(){
    printf("KiadasEdit\n");
}



void kiadasList(){

    headerPrint("KIADAS LISTAZASA");
    

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        
        int index = 1;

        while ((dir = readdir(d)) != NULL) {

            int year;
            int month;

            if (sscanf(dir->d_name, "%d_%d", &year, &month) == 2)
            {
                printf("%d. %s\n", index,dir->d_name);
                index++;
            }

        }
        closedir(d);
    }
    
    printf("Melyik honap kiadasait szeretned kilistazni? ");

    int kiadasIndex = 0;
    char fileName[8];

    scanf("%d", &kiadasIndex);

    d = opendir(".");
    if (d) {
        
        int index = 0;

        while ((dir = readdir(d)) != NULL) {

            int year;
            int month;

            if (sscanf(dir->d_name, "%d_%d", &year, &month) == 2)
            {
                index++;
                if (index == kiadasIndex)
                {   
                    strcpy(fileName, dir->d_name);
                }
            }

        }
        closedir(d);
    }

    printf("Filename : %s\n", fileName);


    FILE* fp = fopen(fileName, "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL)
    {
        printf("Nem sikerult bep");
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
    }

    difftime(mozgo -> datu, dada.datum);



    fclose(fp);
    if (line)
        free(line);
}