#include <unistd.h> // Дополнительные стандартные функции
#include <stdio.h> // Стандартный заголовочный файл
#include <dirent.h> // Для работы с файловой системой
#include <string.h> // Для работы со строками
#include <sys/stat.h> // Для работы с файловой системой
#include <stdlib.h> // Для работы функции strtol

int count = 0; // Индекс строки в массиве   

void print_text(char *dir, char text[][1000]) { // Нахождение файлов, чтение строк из файлов и запись строк в массив
    DIR *dp;
    struct dirent *entry; // Структура для работы с файловой системой
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL) { //Поверка наличия каталогов.
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
    chdir(".."); //Переход в каталог на уровень вверх.
    closedir(dp);
}

void write_to_file(char text[][1000], long int idx[], int size) {
  FILE * fp;
  fp = fopen ("./lab3/result.txt","w");
  int c = 0;
  // long int ix = -99999999999;
  // while (c<count) {
    for (int i=0; i<count; i++) {
      // if (idx[i]==ix) {
         fprintf (fp, "%s\n", text[i]);
         //ix++;
         // c++;
      //}
    }
    //ix++;
  //}
  fclose (fp);
}

int main() {

    char text[1000][1000];
    long int idx[1000];
    long int tmp_idx;
    char tmp_text[1000]; 
    print_text("./", text);
    char * pEnd;
    for (int i=0; i<count; i++) {
        idx[i] = strtol(text[i], &pEnd, 10);
    }
    // Отсортируем массивы
    int c = count-1;
    for (int i=0; i<c; i++)
        for (int j=c; j>i; j--)
            if (idx[j-1]>idx[j]) {
               tmp_idx = idx[j-1];
               for (int k=0; k<1000; k++) 
                  tmp_text[k] = text[j-1][k];
               idx[j-1] = idx[j];
               for (int k=0; k<1000; k++)
                  text[j-1][k] = text [j][k];
               idx[j] = tmp_idx;
               for (int k=0; k<1000; k++)
                  text[j][k] = tmp_text[k]; 
            }
    write_to_file(text, idx, count);
    return 0;
}
