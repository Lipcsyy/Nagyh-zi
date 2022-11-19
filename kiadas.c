#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>

#include "debugmalloc.h"
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
        scanf(" %[^\n]s", kiadasPt->nev);
        printf("\033[A\33[2K\033[A\33[2K");

        printf("Add meg a tetel arat!\n");
        scanf("%d",&(kiadasPt->osszeg));

        printf("\033[A\33[2K\033[A\33[2K");

        printf("Add meg a kategoriajat!\n");
        for (int i = 0; i < 6; i++)
        {
            printf("%d:%s  ",i+1,tags[i]);
        }

        printf("\n");

        while (scanf("%d", &(kiadasPt->kategoria)) == 1 && (kiadasPt->kategoria > 6 || kiadasPt->kategoria <= 0 ))
        {
            printf("Add meg a kategoriajat!\n");
            printf("\033[A\33[2K\033[A\33[2K");
        }   

        printf("\033[A\33[2K\033[A\33[2K");
        printf("\033[A\33[2K\033[A\33[2K");

        //--------------------------------------------láncolt listával

        listaVegFuz(&eleje, kiadasPt);

        kiadasokCount++;
        
        printf("\nSzeretned folytatni? (I) Igen (N) Nem : ");
        canContinue = scanf(" %c", &input) == 1 && (input == 'i' || input == 'I');
        
        free(kiadasPt);

    }

    //összeszámoljuk, mennyit költene összesen most a felhasználó

    int kiadasSum = 0;
    listaElem* mozgo = eleje;

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

        while (mozgo != NULL)
        {
            printf("Sikeres bevitel: %s - %d HUF\n", mozgo->kiadas->nev, mozgo->kiadas->osszeg);
            mozgo = mozgo->kov;
        }

        szamla -= kiadasSum; //a szamlabol kivonom az osszeget

        printf("Szamlawrite elott \n");
        szamlaWriter(szamla); //eltarolom az uj egyenleget
        printf("Szamlawrite utan \n");   

        //itt alakítom ki a filenak a nevét

        char fYear[5];
        sprintf(fYear,"%d", eleje->kiadas->datum.tm_year);

        char fMonth[3];
        sprintf(fMonth,"%d",eleje->kiadas->datum.tm_mon);

        char fileName[11];
        strcpy(fileName, fYear);
        strcat(fileName,"_");
        strcat(fileName,fMonth);

        kiadasWriter(fileName, eleje, "a");
        free(kiadasok);
    }

    //felszabadítom a lancolt listat a fileba írás után

    listaFelszabadit(eleje);


    footerPrint("KIADAS BEVITELE");
    
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


void kiadasWriter(char* fileName, listaElem* eleje, const char* mode)
{

    listaElem* head = eleje;

    if (fileExists(fileName)) //ha létezik akkor csak appendelek a végére
    {
        FILE* fp = fopen(fileName, mode);
        while (head != NULL)
        { 
            fprintf(fp,"%s_%d_%s_%d\n", head->kiadas->nev, head->kiadas->osszeg,tags[head->kiadas->kategoria-1], head->kiadas->id);  
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
        free(head);
        head = temp;
    }
    
}


void kiadasEdit(){
    
    headerPrint("KIADAS MEGVALTOZTATASA");

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    int index = 0;
    if (d) {
    
        while ((dir = readdir(d)) != NULL) {

            int year;
            int month;

            if (sscanf(dir->d_name, "%d_%d", &year, &month) == 2)
            {
                index++;
                printf("%d. %s\n", index,dir->d_name);
            }

        }
        closedir(d);
    }

    printf("\nMelyik honap kiadasait szeretned kilistazni: ");

    int kiadasIndex = 0;
    char fileName[8];

    if(scanf("%d", &kiadasIndex ) == 1)
    {
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

            printf("\nBeolvasott honap: %s\n", fileName);

            FILE* fp = fopen(fileName, "r");
            char * line = NULL;
            size_t len = 0;
            ssize_t read;

            printf("\n");
            if (fp == NULL)
            {
                printf("Nem sikerult beolvasni");
            }

            listaElem* eleje = NULL; 
            int lineIndex = 0;

            while ((read = getline(&line, &len, fp)) != -1) {

                Kiadas* kiadasPt = (Kiadas*) malloc(sizeof(Kiadas));
                char kategoria[20];

                sscanf(line, "%[^_]_%d_%[^_]_%d", kiadasPt->nev, &(kiadasPt->osszeg), kategoria, &(kiadasPt->id));
                
                //printf("%s %d %s %d\n", kiadasPt->nev, kiadasPt->osszeg, kategoria, kiadasPt->id);

                //tags-ből a kategóriának index megkeresese
                for (int i = 0; i < 6; i++)
                {
                    if(strcmp(tags[i], kategoria) == 0 ) kiadasPt->kategoria = i;
                }
                
                listaVegFuz(&eleje, kiadasPt);

                printf("%d. %s", lineIndex ,line);
               
                lineIndex++;
            }


            lineIndex = 0;
            int deleteIndex = 0;

            printf("Melyik elemet szeretned torolni? ");
            scanf("%d", &deleteIndex);
            printf("\n");

            listaElem* head = eleje;
            listaElem* temp = eleje;
            bool deleted = false;

            while (head != NULL && !deleted) 
            {   
                if(deleteIndex == lineIndex)
                {
                    temp->kov = head->kov;
                    free(head);
                    deleted = true;
                }

                temp = head;
                head = head->kov;

                lineIndex++;
            }

            head = eleje;

            while (head != NULL)
            {
                printf("%s %d %s %d \n", head->kiadas->nev,head->kiadas->osszeg, tags[head->kiadas->kategoria], head->kiadas->id);
                head = head->kov;
            }

            kiadasWriter(fileName,eleje, "w");
            
            //TODO: a torolt összeget hozzáadni a számlához.

            listaFelszabadit(eleje);

            fclose(fp);
            if (line)
                free(line);
            
        }

        
    }

    footerPrint("KIADAS MEGVALTOZTATASA");

}


void kiadasList(){

    headerPrint("KIADAS LISTAZASA");
    

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    int index = 0;
    if (d) {
    
        while ((dir = readdir(d)) != NULL) {

            int year;
            int month;

            if (sscanf(dir->d_name, "%d_%d", &year, &month) == 2)
            {
                index++;
                printf("%d. %s\n", index,dir->d_name);
            }

        }
        closedir(d);
    }
    
    printf("\nMelyik honap kiadasait szeretned kilistazni: ");

    int kiadasIndex = 0;
    char fileName[8];



    while (scanf("%d", &kiadasIndex ) == 1 && kiadasIndex != EOF && kiadasIndex <= index)
    {
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

        printf("\nBeolvasott honap: %s\n", fileName);

        FILE* fp = fopen(fileName, "r");
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        printf("\n");
        if (fp == NULL)
        {
            printf("Nem sikerult beolvasni");
        }


        //TODO space kiveszi
        while ((read = getline(&line, &len, fp)) != -1) {
            printf("%s", line);
        }


        fclose(fp);
        
        printf("\nAdd meg melyik tovabbi honapot szeretned kilistazni? ");

    }

    footerPrint("KIADAS LISTAZASA");

}
