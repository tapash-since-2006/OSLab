#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

#define PRINTERS 3

typedef struct{
  int studentId;
  int pages;
}Student;

sem_t printers;



void* printJob(void* arg){
  Student *s=(Student*) arg;

  printf("Student %d requesting printer\n", s->studentId);
  sem_wait(&printers);

  printf("Student %d printing %d pages\n", s->studentId, s->pages);

  sleep(s->pages / 2); // simulate printing time
  printf("Student %d finished printing and released printer\n", s->studentId);

  sem_post(&printers); // release printer
  return NULL;

}

int main(int argc, char*argv[]){
  int n=(argc-1)/2;
  Student *students=malloc(n*sizeof(Student));

  for(int i=0;i<n;i++){
    students[i].studentId=atoi(argv[1+i*2]);
    students[i].pages=atoi(argv[2+i*2]);
  }

  sem_init(&printers, 0, PRINTERS);

  pthread_t threads[n];
  for(int i=0;i<n;i++){
    pthread_create(&threads[i], NULL, printJob, &students[i]);
    usleep(100000);
  }

  for(int i=0;i<n;i++){
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&printers);
  free(students);

  return 0;

}