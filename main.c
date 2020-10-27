#include "main.h"
//parcourer les tags d'un fichier et traiter selon l'action
      int ParcourTags(char* file,char* tag,int action)
       {
           ssize_t buflen, keylen, vallen;
           char *buf, *key, *val;
           buflen = listxattr(file, NULL, 0);
           if (buflen == -1) {
               return 0;
           }
           if (buflen == 0) {
               return 0;
           }
           buf = malloc(buflen);
           if (buf == NULL) {
               return 0;
           }
           buflen = listxattr(file, buf, buflen);
           if (buflen == -1) {
               return 0;
           }
           key = buf;
           while (buflen > 0) {
               vallen = getxattr(file, key, NULL, 0);
               if (vallen == -1)
                   perror("getxattr");

               if (vallen > 0) {
                   val = malloc(vallen + 1);
                   if (val == NULL) {
                       return 0;
                   }
                   vallen = getxattr(file, key, val, vallen);
                   if (vallen == -1)
                       perror("getxattr");
                   else {
                       val[vallen] = 0;
                        if(!strcmp(tag,val)) {
                            if(action!=REMOVE) return 1; 
                            else if(action==REMOVE){
                                int rc = removexattr(file ,key);
                                if(rc < 0) perror(file);  
                                else return 1;         
                            }  
                        }            
                   }

                   free(val);
               } else if (vallen == 0)
                   printf("<no value>");

               keylen = strlen(key) + 1;
               buflen -= keylen;
               key += keylen;
           }
            if(action==EXIST2){
                if(existFils(tag,file)){
                return 1;
                }
            }
           free(buf);
           return 0;
       }
//afficher les tags d'un fichier
       int getTags(char* file)
       {
           ssize_t buflen, keylen, vallen;
           char *buf, *key, *val;
           buflen = listxattr(file, NULL, 0);
           if (buflen == -1) {
               perror("listxattr");
               return 0;
           }
           if (buflen == 0) {
               printf("%s has no attributes.\n", file);
               return 0;
           }
           buf = malloc(buflen);
           if (buf == NULL) {
               perror("malloc");
               return 0;
           }
           buflen = listxattr(file, buf, buflen);
           if (buflen == -1) {
               perror("listxattr");
               return 0;
           }
           key = buf;
           while (buflen > 0) {
               vallen = getxattr(file, key, NULL, 0);
               if (vallen == -1)
                   perror("getxattr");

               if (vallen > 0) {
                   val = malloc(vallen + 1);
                   if (val == NULL) {
                       perror("malloc");
                       return 0;
                   }
                   vallen = getxattr(file, key, val, vallen);
                   if (vallen == -1)
                       perror("getxattr");
                   else {
                       val[vallen] = 0;
                       printf("%s", val);
                   }

                   free(val);
               } else if (vallen == 0)
                   printf("<no value>");

               printf("\n");
               keylen = strlen(key) + 1;
               buflen -= keylen;
               key += keylen;
           }

           free(buf);
           return 0;
       }

//Ajouter un tag à un fichier
int setTag (char* file , char* tag) {
  if(!ParcourTags(file,tag,EXIST)){
  int buflen;
  char *buf;
  buflen = listxattr(file, NULL, 0);
  if (buflen == -1) {
    perror("listxattr");
    exit(EXIT_FAILURE);
  }
  buf = malloc(30);
  sprintf(buf,"user.Tag%d",buflen);
  const char* attr =buf;
  int rc = setxattr(file, attr, tag, strlen(tag), 0);
  if(rc < 0)
    perror(file);
  return !!rc;
  }
  else{
    printf("tag already existing in this file\n");
    return 0;
  }
}
//Supprimmer un tag  d'un fichier
int removeTag (char* file , char* tag) {
  return ParcourTags(file,tag,REMOVE) ;  
}

//recherche selon combinaison des tags
char *substring(char* str,int i, int j){
    char * s = malloc(sizeof(str));
    strcpy(s,str+i);
    s[strlen(s)-j]='\0';
    return s;
}
int exist(int i, int* tab,int len){
    for(int j=0;j<len;j++){
        if (tab[j]==i) return 1;
    }
    return 0;
}
void ListerFichiersDir(char* dir,char* tags)
{
        char* directory = malloc(1000);
        char* directory2 = malloc(1000);
        strcpy(directory,dir);
        strcpy(directory2,directory);
    if(check(tags)){
        DIR *mydir;
        int tab[1000];
        int p=0;
        struct dirent *myfile;
        mydir = opendir(dir);
        int result;
        while((myfile = readdir(mydir)) != NULL)
        {
        directory = malloc(1000);
        strcpy(directory,directory2);
        char* rech = strcat(strcat(directory,"/"),myfile->d_name);
        if(strcmp(myfile->d_name,".") && strcmp(myfile->d_name,"..")){
            struct stat s;
            result = stat(rech, &s); //result 0
            if(!exist(s.st_ino,tab,p)) {
                    tab[p++] = s.st_ino;
                    if(filter(rech,tags)){
                        printf(" %s\n", myfile->d_name);
                    }
                }
            }
            
        }
        closedir(mydir); 
    }
    else {
        printf("combinaison mal tapée\n");
    }
}
int check(char* tags){ // a ameliorer
char* copy = malloc(1000);
strcpy(copy,tags);
char * token = strtok(copy, " ");
if(token==NULL) return 0;
while( token != NULL ) {
   if(token[0]=='"' && token[strlen(token)-1]=='"'){
    }   
    else if (token[0]=='p' && token[1]=='a' && token[2]=='s' && token[3]=='(' && token[4]=='"' && token[strlen(token)-2]=='"' && token[strlen(token)-1]==')'){
    }
    else if (!strcmp("et",token)){
    }
    else {
        return 0;
    }
    token = strtok(NULL, " ");
}
return 1;
}
int filter(char* file,char* tags){
    char* copy = malloc(1000);
    strcpy(copy,tags);
    char * token = strtok(copy, " ");
    while( token != NULL ) {
        if(token[0]=='"'){
            if(!ParcourTags(file,substring(token,1,1),EXIST2)) 
            {
                return 0;
            }
            }   
            else if (token[0]=='p'){
                if(ParcourTags(file,substring(token,5,2),EXIST)){
                     return 0;
                }
            }
            token = strtok(NULL, " ");
    } 
    return 1;
}

int definerPere(char* fils,char* pere){
    FILE* fp = fopen("tags","ab+");
    fseek(fp,0,SEEK_SET);
    tagS buf ;
    while (fread(&buf,sizeof(tagS),1,fp)){
        if (!strcmp(buf.tag,pere)){
            strcpy(buf.fils[buf.nbFils],fils);
            buf.nbFils++;
            fseek(fp,-sizeof(tagS),SEEK_CUR);
            fwrite(&buf,sizeof(tagS),1,fp);
            fclose(fp);
            return 1;
        }
    }
    strcpy(buf.tag,pere);
    strcpy(buf.fils[0],fils);
    buf.nbFils = 1;
    fwrite(&buf,sizeof(tagS),1,fp);
    fclose(fp);
    return 1;
}

int existFils(char* tag,char* file){
    FILE* fp = fopen("tags","rb");
    if(fp==NULL) return 0;
    tagS buf ; 
    while (fread(&buf,sizeof(tagS),1,fp))
    {
        if (!strcmp(buf.tag,tag)){
            for(int i=0;i<buf.nbFils;i++){
                if (ParcourTags(file,buf.fils[i],EXIST) || existFils(buf.fils[i],file)) {
                    fclose(fp);
                    return 1;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

int lien(char* tag,char* tag2,char** hierach){
    FILE* fp = fopen("tags","rb");
    if(fp==NULL) return 0;
    tagS buf ; 
    while (fread(&buf,sizeof(tagS),1,fp))
    {
        if (!strcmp(buf.tag,tag)){
            for(int i=0;i<buf.nbFils;i++){
                if (!strcmp(buf.fils[i],tag2) || lien(buf.fils[i],tag2,hierach)) {
                    if (strcmp("",*hierach)) *hierach = strcat(strcat(*hierach," --> "),buf.tag);
                    else *hierach = buf.tag;
                    fclose(fp);
                    return 1;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

void lienHierachique(char* tag1,char* tag2){
char* s = malloc(1000) ;
strcpy(s,"");
if (lien(tag1,tag2,&s)){
    char* p = malloc(1000) ;
    strcpy(p,tag2);
    p=strcat(p," --> ");
    s=strcat(p,s);
    printf("le lien est : %s\n",p);
}
else {
    s = malloc(1000) ;
    strcpy(s,"");
    if (lien(tag2,tag1,&s)){
    char* p = malloc(1000) ;
    strcpy(p,tag1);
    p=strcat(p," --> ");
    s=strcat(p,s);
    printf("le lien est : %s\n",p);
    }
    else {
        printf("il y'a pas un lien entre %s et %s\n",tag1,tag2);
    }
}
}
int main (int argc,char** argv) {
    if(argc>1){
        if(!strcmp(argv[1],"-t")){
            setTag(argv[2],argv[3]);
        }
        else if(!strcmp(argv[1],"-r")){
            removeTag(argv[2],argv[3]);
        }
        else if(!strcmp(argv[1],"-l")){
            getTags(argv[2]);
        }
        else if(!strcmp(argv[1],"-f")){
            definerPere(argv[2],argv[3]);
        }
        else if(!strcmp(argv[1],"-re")){
            char* s = malloc(10000);
            fgets(s,10000,stdin);
            s[strlen(s)-1]='\0';
            ListerFichiersDir(argv[2],s);
        }
        else if(!strcmp(argv[1],"-lh")){
            lienHierachique(argv[2],argv[3]);
        }
        else{}
    }
    return 0;
}
