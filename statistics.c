#include <stdio.h>
#include <time.h>
#include <dirent.h>

#include "statistics.h"
#include "listManip.h"
#include "debugmalloc.h"
#include "print.h"


void statistics(time_t t){
    headerPrint("STATISZTIKAK");

    //aktuális fájlnév megszerzése

    struct tm datum = *localtime(&t);
    datum.tm_year += 1900;
    datum.tm_mon += 1;

    char fYear[5];
    sprintf(fYear,"%d", datum.tm_year);

    char fMonth[3];
    sprintf(fMonth,"%d",datum.tm_mon);

    char fileName[11];
    strcpy(fileName, fYear);
    strcat(fileName,"_");
    strcat(fileName,fMonth);

    //fájl beolvasás és összeg elmentés

    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if (d) {
        
        listaElem* eleje = NULL;

        int sum = 0;
        while ((dir = readdir(d)) != NULL) {
            
            char sor[200];

            if (strcmp(dir->d_name, fileName) == 0) 
            {   
                FILE * fp = fopen(fileName, "r");
                int osszeg;
                
                while (fgets (sor , 200 , fp) != NULL)
                { 

                    listaElem* kiadas = (listaElem*) malloc(sizeof(listaElem));
                    char kategoria[50]; // ez egy placeholder

                    int foundItems = sscanf(sor, "%[^_]_%d_%[^_]_%d",kiadas->kiadas->nev, &(kiadas->kiadas->osszeg) , kategoria , &(kiadas->kiadas->id));

                    printf("SSCANF értéke %d\n",foundItems);

                    if(listaTartalmaz(eleje, nev) == false)
                    {
                        listaVegFuz(*eleje)
                    }

                    if (foundItems == 4) 
                    {
                        sum += osszeg;
                    }
                }
            }
        }

        printf("Ebben a honapban elkoltott osszeg: %d HUF\n", sum);

        closedir(d);
    }
    
    //TODO: átlagos havi költés

    int atlagKoltes = 0;
    int honapCount = 0;
    d = opendir(".");

    while ((dir = readdir(d)) != NULL) {
        
        int month;
        int year;

        if (sscanf(dir->d_name, "%d_%d", &year, &month) == 2)
        {
            strcpy(fileName, dir->d_name); //hónap nevét itt szerezzük meg

            FILE *fp = fopen(fileName, "r"); //megnyitjuk a file-t
            char sor[200];

            honapCount++;

            while (fgets (sor , 200 , fp) != NULL)
            {
                int osszeg;
                char nev[50];
                char kategoria[20];
                int id;
                int foundItems = sscanf(sor, "%[^_]_%d_%[^_]_%d",nev, &osszeg, kategoria,&id);
                if (foundItems == 4) 
                    atlagKoltes += osszeg;
            }
        }
    }

    atlagKoltes /= honapCount;
    printf("Havi atlagkoltes: %d\n",atlagKoltes);

    closedir(d);

    //TODO: Legtobbszor vasarolt harom termek

    //létrehozok egy láncolt listát úgy hogy kozben szamolom a countot,a legelso elemet mindi fell kell fűzni

    //TODO: Kategoriakra szétbontás és listázás is függőlegesen

    footerPrint("STATISZTIKAK");
}