// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <semaphore.h>
// #include <pthread.h>
// #include <sys/shm.h>

// #define PRINTERS 3

// typedef struct {
//   int studentID;
//   int pages;
// } Student;

// sem_t printers;

// void* printJob(void* arg) {
//   Student *s = (Student*) arg;

//   printf("Student %d requesting printer...\n", s->studentID);
//   sem_wait(&printers);

//   printf("Student %d printing %d pages...\n", s->studentID, s->pages);
//   sleep(s->pages / 2); // simulate printing time
  
//   printf("Student %d finished printing and released printer.\n", s->studentID);
//   sem_post(&printers); // release printer

//   return NULL;
// }

// int main(int argc, char* argv[]) {
//   if (argc < 2) {
//       printf("Error: Missing number of students.\n");
//       return 1;
//   }

//   int n = atoi(argv[1]);

//   // 1. Get the ALREADY CREATED shared memory using the same key (1234)
//   int shmid = shmget(1234, n * sizeof(Student), 0666);
//   if (shmid == -1) {
//       perror("shmget failed in printerAccess");
//       exit(1);
//   }

//   // 2. Attach to the shared memory
//   Student *shared_students = (Student*) shmat(shmid, NULL, 0);

//   sem_init(&printers, 0, PRINTERS);

//   // 3. Thread logic reads directly from shared memory
//   pthread_t threads[n];
//   for(int i = 0; i < n; i++) {
//     pthread_create(&threads[i], NULL, printJob, &shared_students[i]);
//     usleep(100000); // slight delay to order thread requests
//   }

//   for(int i = 0; i < n; i++) {
//     pthread_join(threads[i], NULL);
//   }

//   // 4. Detach shared memory (Parent handles the destruction)
//   shmdt(shared_students);
//   sem_destroy(&printers);

//   return 0;
// }



#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<pthread.h>
#include<semaphore.h>

#define size 3

typedef struct{
  int studentId;
  int pages;
}Student;

sem_t p;


void* printer(void *arg){
  Student * s=(Student*)arg;
  printf("%d Requesting the printer Access\n", s->studentId);

  sem_wait(&p);
  printf("%d printing the pages\n", s->studentId);
  sleep(s->pages/2);

  printf("%d finished printting and releasing the printer\n", s->studentId);
  sem_post(&p);

  return NULL;
}

int main(int argc, char* argv[]){
  int n=atoi(argv[1]);
  
  int shmid=shmget(1234, n*sizeof(Student), 0666);

  Student *s=(Student*) shmat(shmid, NULL, 0);

  sem_init(&p, 0, size);

  pthread_t threads[n];

  for(int i=0;i<n;i++){
    pthread_create(&threads[i], NULL, printer, &s[i]);
  }

  for(int i=0;i<n;i++){
    pthread_join(threads[i], NULL);
  }
  shmdt(s);
  sem_destroy(&p);

  return 0;
}