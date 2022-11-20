#include <stdio.h>
#include <time.h>
#include <dirent.h>

#include "statistics.h"
#include "debugmalloc.h"
#include "print.h"

typedef struct countOfElem
{
    int count;
    char nev[50];
    struct countOfElem* kov;
} countOfElem;

void statistics(time_t t){
    headerPrint("STATISZTIKAK");

    //TODO: Havi költés

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
        
        countOfElem* eleje = NULL;

        int sum = 0;
        while ((dir = readdir(d)) != NULL) {
            
            char * line = NULL;
            size_t len = 0;
            ssize_t read;

            if (strcmp(dir->d_name, fileName) == 0) 
            {   
                FILE * fp = fopen(fileName, "r");
                int osszeg;
                char nev[50];
                char kategoria[20];
                int id;
                
                while ((read = getline(&line, &len, fp)) != -1)
                { 
                    int foundItems = sscanf(line, "%[^_]_%d_%[^_]_%d",nev, &osszeg, kategoria,&id);
                    if (foundItems == 4) 
                    {
                        printf("%d\n",osszeg);
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
            char * line = NULL;
            size_t len = 0;
            ssize_t read;

            honapCount++;

            while ((read = getline(&line, &len, fp)) != -1)
            {
                int osszeg;
                char nev[50];
                char kategoria[20];
                int id;
                int foundItems = sscanf(line, "%[^_]_%d_%[^_]_%d",nev, &osszeg, kategoria,&id);
                if (foundItems == 4) 
                    atlagKoltes += osszeg;
            }
        }
    }

    atlagKoltes /= honapCount;
    printf("Havi atlagkoltes: %d\n",atlagKoltes);

    closedir(d);

    //TODO: Legtobbszor vasarolt harom termek

    //TODO: Kategoriakra szétbontás és listázás is függőlegesen

    footerPrint("STATISZTIKAK");
}