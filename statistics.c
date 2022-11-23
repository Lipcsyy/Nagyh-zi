#include <stdio.h>
#include <time.h>
#include <dirent.h>

#include "statistics.h"
#include "listManip.h"
#include "debugmalloc.h"
#include "print.h"

char* Tags[] = {"vendeglatas","szepsegapolas","ruhazat", "szorakozas", "bevasarlas", "egyeb" };

void statistics(time_t t)
{
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
        
        countOfElem* eleje = NULL;
        int kategoriaSum[6] = {0,0,0,0,0,0};

        int sum = 0;

        while ((dir = readdir(d)) != NULL) {
            
            char sor[200];

            if (strcmp(dir->d_name, fileName) == 0) 
            {   
                FILE * fp = fopen(fileName, "r");
                int osszeg;
                
                while (fgets (sor , 200 , fp) != NULL)
                { 
                    
                    char nev[50];
                    char* kategoria = (char*) malloc(sizeof(char)*20); // ez egy placeholder
                    int osszeg = 0;
                    int id = 0;

                    int foundItems = sscanf(sor, "%[^_]_%d_%[^_]_%d", nev, &osszeg, kategoria, &id);
                    
                    if (foundItems == 4) 
                    {   
                        sum += osszeg;

                        if(listaTartalmaz(eleje,nev) == false)
                        {
                            statListaVegFuz(&eleje, nev); //hogyha nem tartalmazza, akkor hozzafuzzuk, ha pedig tartalmazza akkor a listaTartalmaz fuggveny noveli a szamat automatikusan
                        }

                        for (int i = 0; i < 6 ; i++)
                        {   
                            if(strcmp(kategoria, Tags[i]) == 0)
                            {   
                                kategoriaSum[i] += osszeg; 
                            }
                        }

                    }

                    free(kategoria);
                }  

                //maximum megkeresese 
                countOfElem* head = eleje;

                countOfElem* maxElem = NULL;
                int countMax = 0;

                while (head != NULL)
                {
                    if (head->count > countMax)
                    {
                        countMax = head->count;
                        maxElem = head;
                    }
                    
                    head = head->kov;
                }
                
                printf("Ebben a honapban a legtobbet vasarolt elem a(z) %s volt, ezt osszesen %d alkalommal vasaroltad meg.\n",maxElem->nev, maxElem->count);
                
                    //TODO: Kategoriakra szétbontás és listázás is függőlegesen

                for (int i = 0; i < 6; i++)
                {
                    printf("%s - %d HUF\n", Tags[i], kategoriaSum[i]);
                }

                statListaFelszabadit(eleje);
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
    
    footerPrint("STATISZTIKAK");
}