#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"
void ForDirectory(char *Source, char *Dest, char filepath[]);
void Display(char *path);
void CopyingFiles(char *Arg1, char filepath[], char *Arg2);
void MakeDestinationPath(char *Arg1, char *Arg2, char filepath[], char name[]);
char *GetName(char *f, char name[]);
int main(int argc, char *argv[]) {
  if (argc > 4 || argc < 3) {
    printf("Not A correct way to use copy fucntion \n");
    return 1;
  }
  char filepath[1024];
  char name[1024];
  if (argc == 3) {
    MakeDestinationPath(argv[1], argv[2], filepath, name);
    CopyingFiles(argv[1], filepath, argv[2]);
  } else if (argc == 4) {
    if ((strcmp(argv[1], "-r")) != 0) {
      printf("Not a correct Option !\n");
      return 2;
    } else {
      char *DirecName = GetName(argv[2], name);
      strcpy(filepath, argv[3]);
      strcat(filepath, "/");
      strcat(filepath, DirecName);
      mkdir(filepath, 0777);
      ForDirectory(argv[2], argv[3], filepath);
    }
  }
}
char *GetName(char *f, char name[]) {
  char *i;
  char *N = name;
  if ((i = strrchr(f, '/')) != NULL) {
    while (*(i + 1)) {
      *N = *(i + 1);
      i++;
      N++;
    }
    *N = '\0';
    return name;
  } else
    return f;
}
void MakeDestinationPath(char *Arg1, char *Arg2, char filepath[], char name[]) {
  strcpy(filepath, Arg2);
  char *P = GetName(Arg1, name);
  strcat(filepath, "/");
  strcat(filepath, P);
  printf("%s\n", filepath);
}
void CopyingFiles(char *Arg1, char filepath[], char *Arg2) {
  printf(" source is %s and path is %s\n", Arg1, filepath);
  FILE *Sfptr = fopen(Arg1, "rb");
  if (Sfptr == NULL) {
    printf("Cant open file to read \n");
    return;
  }
  FILE *Dfptr = fopen(filepath, "wb");
  if (Dfptr == NULL) {
    printf("Cant open file to write \n");
    return;
  }
  char arr[10000];
  int bytesRead;
  while ((bytesRead = fread(arr, 1, sizeof(arr), Sfptr)) > 0) {
    fwrite(arr, 1, bytesRead, Dfptr);
  }
  fclose(Sfptr);
  fclose(Dfptr);
  Display(Arg1);
}
void ForDirectory(char *Source, char *Dest, char filepath[]) {
  DIR *Dptr = opendir(Source);
  if (!Dptr)
    return;
  char tempath[1024];
  strcpy(tempath, filepath);
  strcat(tempath, "/");
  struct dirent *elementInfo;
  while ((elementInfo = readdir(Dptr)) != NULL) {
    if ((strcmp(elementInfo->d_name, ".") == 0) ||
        (strcmp(elementInfo->d_name, "..") == 0))
      continue;
    if (elementInfo->d_type == DT_DIR) {
      char TdirectSrc[1024];
      char Tdirectpath[1024];
      strcpy(TdirectSrc, Source);
      strcpy(Tdirectpath, filepath);
      strcat(TdirectSrc, "/");
      strcat(Tdirectpath, "/");
      strcat(TdirectSrc, elementInfo->d_name);
      strcat(Tdirectpath, elementInfo->d_name);
      mkdir(Tdirectpath, 0777);
      ForDirectory(TdirectSrc, Dest, Tdirectpath);
    } else {
      char tempsrc[1024];
      char Tpath[1024];
      strcpy(Tpath, tempath);
      strcat(Tpath, elementInfo->d_name);
      strcpy(tempsrc, Source);
      strcat(tempsrc, "/");
      strcat(tempsrc, elementInfo->d_name);
      CopyingFiles(tempsrc, Tpath, Dest);
    }
  }
  closedir(Dptr);
}
void Display(char *path) {
  printf(RED
         "*" GREEN
         "---------------------------------------------------------------" RESET
         "\n");
  printf("Copied %s\n", path);
  printf(RED
         "*" GREEN
         "---------------------------------------------------------------" RESET
         "\n");
  printf("\n\n");
}
