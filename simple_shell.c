#include <stdio.h>
#include<sys/stat.h>
#include<sys/types.h>/*are nevoie systat.h de anumite tipuri de var*/
#include<stdlib.h>/*malloc*/
#include<string.h>
#include <unistd.h>/*pentru pwd,mkdir,chdir,etc*/
#include <limits.h>/*PATH_MAX*/
#include <dirent.h>//asta te lasa sa deschizi/inchizi/orice alta comanda cu directoarele
#include<conio.h>
#include "header.h"



int main()
{

    system("cls");
    char *propozitie=calloc(1000,sizeof(char));
    

    

    while(1){

        char path[PATH_MAX];
        printf("%s >> ",getcwd(path,PATH_MAX));
        
        memset(propozitie,0,1000);

        citire(propozitie);


        if(strcmp(propozitie,"\n")!=0)
        {
            int er;
            char **cuvinte=malloc(10*sizeof(char*));
            int cnt=0;

            for(int i=0;i<10;i++)
            {
                cuvinte[i]=malloc(sizeof(char)*60);

            }
            
            char *p=strtok(propozitie," \n");

            while(p!=NULL)
            {
                strcpy(cuvinte[cnt++],p);

                p=strtok(NULL," \n");


            }
            
            if(strcmp(cuvinte[0],"clear")==0)
                system("cls");
            else 
                if(strcmp(cuvinte[0],"stop")==0)
                    break;
                else
                {
                    er=analizeaza_comanda(cuvinte,cnt);
                    if(er==0)
                    printf(RED"UNKNOWN COMMAND\n"RESET);
                }
        }   

        }
        
        /// sscanf() alege de unde citeste si unde pune
        
    return 0;
}