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



void continut(char *nume)
{

FILE *deschide;
    
    deschide=fopen(nume,"r");

    if(deschide==NULL)
    {

        printf(RED"EROARE LA DESCHIDEREA FISIERULUI// ASIGURATI-VA CA FISIERUL ESTE DE TIP TEXT\n"RESET);
    }

    else{

        printf("a mers\n");

        long dimensiune_fisier;

        fseek(deschide, 0, SEEK_END);//mergi la capat de fisier

        dimensiune_fisier=ftell(deschide);//iti zice in biti ce dimensiune are

        rewind(deschide);//te intoarece la inceput de tot

        char *continut=malloc(dimensiune_fisier);

        if(continut!=NULL)
        {

            fread(continut,1,dimensiune_fisier,deschide);//mai primitiv decat fgets plus ca fgets citeste pana la \n 
        

            continut[dimensiune_fisier]='\n';

            printf("%s",continut);
        }
        
        else
        printf(RED"EROARE LA ALOCAREA MEMORIEI\n"RESET);

        

    }

}



void schimba_dir(char **cuvinte,int cnt)
{

    
    char *vechi=malloc(60);
    vechi=getcwd(vechi,PATH_MAX);

    if(chdir(cuvinte[1])==-1)
    printf(RED"DIRECTORUL "RESET"'%s'"RED" NU EXISTA\n"RESET,cuvinte[1]);

    else
    printf("ati schimbat directorul din:\n ==>%s \nin\n ==> %s\n",vechi,cuvinte[1]);


}



void sterge_recurent(char * director){
    //director este variabila in care se stocheaza adresa directorului parinte

    char cale_completa[1024];

    struct stat info_fisier;//macro
    DIR *directorul_deschis ;

    directorul_deschis=opendir(director);
    

    struct dirent *elemente;

    if(director!=NULL)
    while((elemente=readdir(directorul_deschis))!=NULL){

        int ok=1,k=0;

        snprintf(cale_completa,sizeof(cale_completa),"%s/%s",director,elemente->d_name);//lfl ca strcat doar ca nu mai scrii doua strcaturi
        
        
        if(strcmp(elemente->d_name,".")==0 || strcmp(elemente->d_name,"..")==0)continue;//ignora dir parinte

        if(stat(cale_completa,&info_fisier)==0){
                ok=1;
        }

        if(ok)
        {
            if(S_ISDIR(info_fisier.st_mode))
            {
                sterge_recurent(cale_completa);
            }

            else{
                
                if(remove(cale_completa)==-1)
                printf(RED"Eroare la stergere"RESET);

            } 

        }

        
        
    
    }

    closedir(directorul_deschis);//trebuie inchis inainte sa l stergi pt ca ti blocheaza windowsu actiunea

    if(rmdir(director)!=0)
    {
        printf(RED"Directorul %s nu s a putut sterge\n"RESET,director);
    }

    


}



void sterge(char **cuvinte,int cnt)
{

    
    DIR *directorul_deschis ;
    

    struct dirent *elemente;

    for(int i=1;i<cnt;i++){
        struct stat info_fisier;//macro

        if(stat(cuvinte[i],&info_fisier)==-1)
        {
            printf(RED"EROARE\n"RESET);
            continue;
        }

        if(S_ISDIR(info_fisier.st_mode))
        {

            sterge_recurent(cuvinte[i]);//merge si fara fullpath pentru ca am ** care mi da adresa exacta in pc
        }
        
        else
        {
            if(remove(cuvinte[i])==-1)
            {
                printf(RED"EROARE LA STERGERE\n"RESET);
            }


        }
        


    }



}



void creaza_director(char **cuvinte,int cnt){

    char *optiune=malloc(30);
    

    if(cnt==2){

        strcpy(optiune,cuvinte[1]);
        
        mkdir(optiune);
        printf(GREEN"The directory '%s' has been created with succes!!\n"RESET,optiune);

    }
    
    else
    printf(RED"TOO MANY ARGUMENTS!\n"RESET);


}



int has_extension(char * nume)
{

    char *terminatii[] = {
    // Sistem / C / C++
    ".c",".exe",".h", ".cpp", ".cc", ".cxx", ".hpp", ".hh", ".s", ".asm", ".rs", ".go",
    
    // Web
    ".html", ".htm", ".css", ".js", ".ts", ".php", ".py", ".rb",
    
    // Enterprise / Mobile
    ".java", ".cs", ".kt", ".kts", ".swift", ".m", ".scala",
    
    // Scripting
    ".sh", ".bat", ".cmd", ".ps1", ".lua", ".pl",
    
    // Configurare si Date
     ".xml", ".yaml", ".yml", ".sql"
    };

    int ok=0;
    int nr = sizeof(terminatii) / sizeof(terminatii[0]);

    for(int i=0;i<nr;i++)
    {
        if(strstr(nume,terminatii[i]))
        {
            ok=1;
            break;
        }
    }

    return ok;
}



void afiseaza(char **cuvinte,int cnt)
{
    struct stat info_fisier;//macro

    //dirent.h introduce typeul DIR*  dar si structura dirent
    
    //struct dirent=char d_name;
    
    //opendir returneaza un directory stream care citeste totalitatea directorului si are diferite "entryuri" de care ma folosesc sa accesez elementele din director
    

    char ceva[PATH_MAX];
    if(getcwd(ceva,PATH_MAX)==NULL)return;

    DIR *directorul_deschis ;
    directorul_deschis=opendir(ceva);

    struct dirent *elemente;
    //daca mai ai specificat ce tip de fileuri vrei sa afisezi(t=text,a=apl,d=dir)
    
    if(cnt>2){
        printf(RED"ONE ARGUMENT EXPECTED"RESET);
    }

    else if(cnt==2){

        char optiune=cuvinte[1][0];
        int ok=0;
        char valide[4]={'t','d','a'};
        int cul=3;

        for(int i=0;i<3;i++)
        {
            if(optiune==valide[i])
            ok=1;
        }

    
        if(ok==0)
        printf(RED"UNKNOWN COMMAND"RESET);

        else
        while((elemente=readdir(directorul_deschis))!=NULL)
    {
            
            if(strcmp(elemente->d_name,".")==0 || strcmp(elemente->d_name,"..")==0)continue;//ignora dir parinte
            int scrie;

            if(stat(elemente->d_name,&info_fisier)==0){
                scrie=0;
            }
            
            switch(optiune)
        {

            case 't':
            if(S_ISREG(info_fisier.st_mode) && strstr(elemente->d_name,".txt")){
                scrie=1;
                cul=3;
            }

             if(has_extension(elemente->d_name))
            {
                cul=4;
                scrie=1;
            }

            break;

            case 'a':
            if(S_ISREG(info_fisier.st_mode) && strstr(elemente->d_name,".lnk")){
                scrie=1;
                cul=1;
            }
            break;

            case 'd':
            if(S_ISDIR(info_fisier.st_mode)){
                scrie=1;
                cul=2;
            }
            default:
            break;

        }

        

        if(scrie)printf("%s%s\n%s",paleta[cul].cod_ansi,elemente->d_name,RESET);

    }
        
    }


    else if(cnt==1)
    {
        while((elemente=readdir(directorul_deschis))!=NULL){

             int cul=3;

             if(strcmp(elemente->d_name,".")==0 || strcmp(elemente->d_name,"..")==0)continue;//ignora dir parinte

             int scrie;

            if(stat(elemente->d_name,&info_fisier)==0){
                scrie=1;
            }

            if(S_ISREG(info_fisier.st_mode) && has_extension(elemente->d_name)){

                cul=4;
            }

            if(S_ISREG(info_fisier.st_mode) && strstr(elemente->d_name,".txt")){

                cul=3;
            }


            if(S_ISREG(info_fisier.st_mode) && strstr(elemente->d_name,".lnk")){
                
                cul=1;
            }

            if(S_ISDIR(info_fisier.st_mode)){
                
                cul=2;
            }

               if(scrie==1)printf("%s%s\n%s",paleta[cul].cod_ansi,elemente->d_name,RESET);

        }
        
    }


    
    printf("\n");

}



void create_file(char **cuvinte,int cnt)
{
    for(int i=1;i<cnt;i++)
    {
        FILE *f = fopen(cuvinte[i], "at");
        if (f) {
            fclose(f);
        }

    }

    if(cnt>=3)
    printf(GREEN"FISIERELE AU FOST CREATE CU SUCCES\n"RESET);

    else printf(GREEN"FISIERUL A FOST CREAT CU SUCCES\n"RESET);
    
}



int analizeaza_comanda(char **cuvinte,int cnt)
{
    int valid=0;

    if(strcmp(cuvinte[0],"pwd")==0)
    {
        char ceva[PATH_MAX];
        printf("->%s\n",getcwd(ceva,PATH_MAX));
        valid=1;
    }

    if(strcmp(cuvinte[0],"mkdir")==0)
    {
        creaza_director(cuvinte,cnt);
        valid=1;
    }

    if(strcmp(cuvinte[0],"rm")==0)
    {
        sterge(cuvinte,cnt);

        valid=1;
    }

    if(strcmp(cuvinte[0],"cd")==0)
    {
        schimba_dir(cuvinte,cnt);

        valid=1;
    }

    if(strcmp(cuvinte[0],"ls")==0)
    {
        afiseaza(cuvinte,cnt);

        valid=1;
    }

    if(strcmp(cuvinte[0],"touch")==0)
    {
        create_file(cuvinte,cnt);

        valid=1;
    }

    if(strcmp(cuvinte[0],"content")==0)
    {
        continut(cuvinte[1]);
        valid=1;
    }
    

    

    return valid;

}



void completeaza(char * propozitie,int *i)
{
    // char cuvant[10]="david";
    // strcpy(propozitie+*i,cuvant);
    // printf("%s",cuvant);
    // *i=*i+strlen(cuvant);
    // fflush(stdout);


    char completeaza[20];
    int lungime_prefix;
    int poz;
    for(poz=*i;poz>=0;poz--)
    {
        if(propozitie[poz]==' ')
        {
            poz=poz+1;
            break;
        }
    }

    //printf("\n\n%s",propozitie+poz);

    char*prefix;
    prefix=propozitie+poz;
    lungime_prefix=strlen(prefix);

    //DESCHIDEM FOLDERUL CURENT

    const char *dir_cur=".";
    int gasite=0;
    char **lafel=malloc(sizeof(char*)*20);

    for(int n=0;n<20;n++)
    {
        lafel[n]=malloc(40*sizeof(char));
    }

    struct stat info_fisier;
    DIR *director_deschis;
    director_deschis=opendir(dir_cur);

    struct dirent *elemente;

    while((elemente=readdir(director_deschis))!=NULL)
    {
        if(strcmp(elemente->d_name,".")==0 || strcmp(elemente->d_name,"..")==0)continue;// fara parinte

        if(stat(elemente->d_name,&info_fisier)==0 && strstr(elemente->d_name,prefix))
        {
            strcpy(lafel[gasite++],elemente->d_name);
        }

    }

    //VERIFIC SA VAD DACA A GASIT ASEMANATOARE;

    // for(int k=0;k<gasite;k++)
    // {
    //     printf("%d) %s\n",k+1,lafel[k]);
    // }
    
    closedir(director_deschis);

    int k=0,tab_apasat=0;
    int lungime_sufix_vechi=0;

    while(1)
    {
        char *nume_complet=lafel[abs(k)%gasite];
        char *sufix_nou=nume_complet+lungime_prefix;


        //AFISAM SUFIXU DIN LAFEL

        printf("%s",sufix_nou);

        char optiune=_getch();
        lungime_sufix_vechi=strlen(sufix_nou);

        

        fflush(stdout);


            
        if(optiune=='p')
        {
            k++;//sageaata dreapta
            for (int n = 0; n < lungime_sufix_vechi; n++)
            {
            printf("\b \b");
            }
        }
        

        else if(optiune=='o'){

            k--;//sageaata stanga
            for (int n = 0; n < lungime_sufix_vechi; n++)//STERGE SUFIX/ MUTA CURSORU LA STANGA 
            {
            printf("\b \b");
            }
        }
        


        else//DACA NU ESTE CARACTER SPECIAL
        {

            strcpy(propozitie+poz,nume_complet);//ACTUALIZAM PROPOZITIA
            *i=poz+strlen(nume_complet);
            
            
            fflush(stdout);

            if(optiune!='\r' && optiune !='\t')//lasa citirea sa continuie
            ungetch(optiune);

            break;
        }

    }

    for(int n = 0; n < gasite; n++) free(lafel[n]);
    free(lafel);

    

}



void citire(char *propozitie)
{

    char x;
    int i=0;
    while(1)
    {
        x=_getch();
        
        if(x=='\t')
        completeaza(propozitie,&i);
        
        if(x=='\r')//getchar nu returneaza \n ci \r
        {
            propozitie[i]='\n';
            printf("\n");
            fflush(stdout);
            break;

        }
        

        if(x>=32){
            propozitie[i++]=x;
            putch(x);
            propozitie[i]='\0';

            
            fflush(stdout);

        }

        if(x==127 ||x==8)
        {
            if(i>0)
            {
                i--;
                propozitie[i]='\0';
                printf("\b \b");
                fflush(stdout);
            }
        }
        

    }

    


}
