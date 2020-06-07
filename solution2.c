#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int count = 0; // Индекс строки в массиве   

void print_text(char *dir, char text[][1000]) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            /* Находит каталог, и делает рекурсивный вызов для следующего поиска */
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
               continue;
            print_text(entry->d_name, text);
        }
        else //В случае, если это файл
        {
            if(strstr(entry->d_name,".txt")){
                FILE *d = fopen(entry->d_name,"r");
                while(!feof(d)) {
                     fgets(text[count], 1000, d);
                }
                count++;
                fclose(d);
            }
       }
    }
    chdir("..");
    closedir(dp);
}

void write_to_file(char text[][1000], long int idx[], int size) {
  FILE * fp;
  fp = fopen ("/home/box/result.txt","w");
  int c = 0;
  int ix = 0;
  while (c<count) {
    for (int i=0; i<count; i++) {
      if (idx[i]==ix) {
         fprintf (fp, "%s\n", text[i]);
         //ix++;
         c++;
      }
    }
    ix++;
  }
  fclose (fp);
}

int main() {

    char text[1000][1000];
    long int idx[1000];
    print_text("./", text);
    char * pEnd;
    for (int i=0; i<count; i++) {
        idx[i] = strtol(text[i], &pEnd, 10);
    }
    write_to_file(text, idx, count);
    return 0;
}
