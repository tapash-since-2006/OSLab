#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

typedef struct{
  int studentID;
  int pages;
}Student;

int main(){
  int n;
  printf("Enter the number of students: ");
  scanf("%d", &n);

  Student *students=malloc(n*sizeof(Student));

  for(int i=0;i<n;i++){
    printf("Enter the Student Id and Pages for the student %d: ", i+1);
    scanf("%d %d", &students[i].studentID, &students[i].pages);
  }

  printf("\nStudentID\tPages\n");
  for(int i=0; i<n; i++) {
    printf("%d\t\t%d\n", students[i].studentID, students[i].pages);
  }

  printf("\nExecuting printerAccess program...\n");

  char* args[2*n+2];
  args[0]="./printerAccess";

  for(int i=0;i<n;i++){
    char *id=malloc(10*sizeof(char));
    char *pages=malloc(10*sizeof(char));

    sprintf(id,"%d", students[i].studentID);
    sprintf(pages, "%d", students[i].pages);

    args[1+i*2]=id;
    args[1+(i*2)+1]=pages;
  }

  args[1+(2*n)]=NULL;

  pid_t pid=fork();

  if(pid==0){
    execv("./printerAccess", args);
    exit(1);
  }else{
    wait(NULL);
  }

  return 0;



}