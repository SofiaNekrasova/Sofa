#include <unistd.h> // �������������� ����������� �������
#include <stdio.h> // ����������� ������������ ����
#include <dirent.h> // ��� ������ � �������� ��������
#include <string.h> // ��� ������ �� ��������
#include <sys/stat.h> // ��� ������ � �������� ��������
#include <stdlib.h> // ��� ������ ������� strtol

int count = 0; // ������ ������ � �������   

void print_text(char *dir, char text[][1000]) { // ���������� ������, ������ ����� �� ������ � ������ ����� � ������
    DIR *dp;
    struct dirent *entry; // ��������� ��� ������ � �������� ��������
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL) { //������� ������� ���������.
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            /* ������� �������, � ������ ����������� ����� ��� ���������� ������ */
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
               continue;
            print_text(entry->d_name, text);
        }
        else //� ������, ���� ��� ����
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
    chdir(".."); //������� � ������� �� ������� �����.
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
    // ����������� �������
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
