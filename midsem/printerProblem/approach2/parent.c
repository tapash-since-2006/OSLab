// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <sys/shm.h>

// typedef struct {
//   int studentID;
//   int pages;
// } Student;

// int main() {
//   int n;
//   printf("Enter the number of students: ");
//   scanf("%d", &n);

//   // 1. Create shared memory block for an array of 'n' Students (using key 1234)
//   int shmid = shmget(1234, n * sizeof(Student), 0666 | IPC_CREAT);
//   if (shmid == -1) {
//     perror("shmget failed");
//     exit(1);
//   }

//   // 2. Attach shared memory to this process
//   Student *shared_students = (Student*) shmat(shmid, NULL, 0);

//   // 3. Write data directly into shared memory
//   for(int i = 0; i < n; i++) {
//     printf("Enter the Student Id and Pages for the student %d: ", i+1);
//     scanf("%d %d", &shared_students[i].studentID, &shared_students[i].pages);
//   }

//   printf("\nStudentID\tPages\n");
//   for(int i = 0; i < n; i++) {
//     printf("%d\t\t%d\n", shared_students[i].studentID, shared_students[i].pages);
//   }

//   printf("\nExecuting printerAccess program...\n");

//   // 4. We only need to pass 'n' to the child program now!
//   char n_str[10];
//   sprintf(n_str, "%d", n);
  
//   char* args[] = {"./pa", n_str, NULL};

//   pid_t pid = fork();

//   if(pid == 0) {
//     execv("./pa", args);
//     perror("execv failed"); // only prints if execv fails
//     exit(1);
//   } else {
//     wait(NULL); // Wait for printerAccess to finish
    
//     // 5. Detach and Destroy shared memory (Important step!)
//     shmdt(shared_students);
//     shmctl(shmid, IPC_RMID, NULL); 
//   }

//   return 0;
// }


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/wait.h>

typedef struct{
  int studentId;
  int pages;
}Student;

int main(){
  int n;
  printf("Enter the number of students \n");

  scanf("%d", &n);

  Student* sh;

  int shmid=shmget(1234, n*sizeof(Student), 0666 | IPC_CREAT);
  sh=(Student*) shmat(shmid, NULL, 0);

  for(int i=0;i<n;i++){
    printf("Enter the id and pages of student no %d: ", i+1);
    scanf("%d %d", &sh[i].studentId, &sh[i].pages);
  }

  for(int i=0;i<n;i++){
    printf("%d\t\t%d\n",sh[i].studentId, sh[i].pages);
  }

  // printer doing its thing

  char *args[3];

  args[0]="./pa";

  // char temp[100];
  // sprintf(temp, "%d", n);
  // args[1]=temp;

  args[1]=malloc(15*sizeof(char));
  sprintf(args[1], "%d", n);
  args[2]=NULL;

  pid_t pid;

  pid=fork();

  if(pid==0){
    execv("./pa", args);
    exit(1);
  }else{
    wait(NULL);
    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);
  }

  return 0;
}
