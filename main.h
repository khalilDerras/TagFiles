#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#define REMOVE 1
#define EXIST 2
#define EXIST2 2

typedef struct tag tagS;
struct tag {
    char tag[1000];
    char fils[1000][1000];
    int nbFils;
};

int ParcourTags(char* file,char* tag,int action);
int getTags(char* file);
int setTag (char* file , char* tag) ;
int removeTag (char* file , char* tag) ;
char *substring(char* str,int i, int j);
void ListerFichiersDir(char* dir,char* tags);
int check(char* tags);
int filter(char* file,char* tags);
int definerFils(char* fils,char* pere);
int existFils(char* tag,char* file);
void *getFils(char* pere);
