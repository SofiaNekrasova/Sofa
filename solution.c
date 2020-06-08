#include <unistd.h> // �������������� ����������� �������
#include <stdio.h> // ����������� ������������ ����
#include <dirent.h> // ��� ������ � �������� ��������
#include <string.h> // ��� ������ �� ��������
#include <sys/stat.h> // ��� ������ � �������� ��������
#include <stdlib.h> // ��� ������ ������� strtol

int count = 0; // ������ ������ � �������   

void print_text(char *dir, char text[5000][80], long int *idx) { // ���������� ������, ������ ����� �� ������ � ������ ����� � ������
    char * pEnd;
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
            print_text(entry->d_name, text, idx);
        }
        else //� ������, ���� ��� ����
        {
            if(strstr(entry->d_name,".txt")){
                FILE *d = fopen(entry->d_name,"r");
                while(!feof(d)) {
                     fgets(text[count], 80, d);
                }
                idx[count] = strtol(text[count], &pEnd, 10);
                count++;
                fclose(d);
            }
       }
    }
    chdir(".."); //������� � ������� �� ������� �����.
    closedir(dp);
}

void write_to_file(char text[5000][80], long int idx[], int size) {
  FILE * fp;
  fp = fopen ("./result.txt","w");
  int c = 0;
  int i = 0;
    for (i=0; i<count; i++) {
         fprintf (fp, "%s\n", text[i]);
    }
  fclose (fp);
}

int main() {

    char text[5000][80]={0};
    long int idx[5000];
    long int tmp_idx;
    char tmp_text[80]={0};
    int i, j, k;
    for (i=0; i<5000; i++)
      idx[i] = 0;
    print_text("./", text, idx);
    // ����������� �������
    int c = count-1;
    for (i=0; i<c; i++)
        for (j=c; j>i; j--)
            if (idx[j-1]>idx[j]) {
               tmp_idx = idx[j-1];
               for (k=0; k<80; k++)
                  tmp_text[k] = text[j-1][k];
               idx[j-1] = idx[j];
               for (k=0; k<80; k++)
                  text[j-1][k] = text [j][k];
               idx[j] = tmp_idx;
               for (k=0; k<80; k++)
                  text[j][k] = tmp_text[k];
            }
    write_to_file(text, idx, count);
    return 0;
}
