
#include <stdio.h>
#include<sys/stat.h>
#include<sys/types.h>/*are nevoie systat.h de anumite tipuri de var*/
#include<stdlib.h>/*malloc*/
#include<string.h>
#include <unistd.h>/*pentru pwd,mkdir,chdir,etc*/
#include <limits.h>/*PATH_MAX*/
#include <dirent.h>//asta te lasa sa deschizi/inchizi/orice alta comanda cu directoarele
#include<conio.h>

//macros pentru culori
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\033[0m"
#define BLUE    "\033[0;34m"

//dictionar basically
typedef struct {
    char* nume;
    char* cod_ansi;
} Culoare;

static Culoare paleta[] = {
    {"rosu", RED},
    {"verde", GREEN},
    {"albastru",BLUE},
    {"alb",RESET},
    {"galben",YELLOW}
};



void continut(char *nume);//ls prints in colors (y-exe | b-dir | w-text)

void schimba_dir(char **cuvinte,int cnt);//chdir

void sterge_recurent(char * director);//rm always removes recurent
void sterge(char **cuvinte,int cnt);

void creaza_director(char **cuvinte,int cnt);//mkdi- n arguments

int has_extension(char * nume);

void afiseaza(char **cuvinte,int cnt);//ls

void create_file(char **cuvinte,int cnt);//touch

int analizeaza_comanda(char **cuvinte,int cnt);

void completeaza(char * propozitie,int *i);//completes words/extensions "tab"

void citire(char *propozitie);



