#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/wait.h>

#define TOTAL_ROOMS 5

sem_t room_semaphore;

struct Student{
  int id;
  int arrival_time;
  int duration;
};


void* allocate(void *arg){
  struct Student* s=(struct Student*)arg;

  sleep(s->arrival_time);

  printf("\n[Time: %ds] Student %d arrived and is requesting a room.\n", s->arrival_time, s->id);
  int availableRooms;

  sem_getvalue(&room_semaphore, &availableRooms);
  
  if(availableRooms<=0){
    printf("\nNo vacancy available. %d is waiting for a room\n", s->id);
  }

  sem_wait(&room_semaphore);


  printf("Student %d has been allocated a room\n", s->id);

  sleep(s->duration);

  printf("Student %d is exiting the room \n", s->id);
  sem_post(&room_semaphore);

  return NULL;
}

int main(){
  int n;
  printf("Enter number of students to allocate: ");
  scanf("%d", &n);

  struct Student* s=(struct Student*) malloc(n*sizeof(struct Student));

  for(int i=0;i<n;i++){
    printf("Enter the student id, arrival_time and the duration");
    scanf("%d %d %d", &s[i].id, &s[i].arrival_time, &s[i].duration);
  }

  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-i-1;j++){
      if(s[j].arrival_time>s[j+1].arrival_time){
        struct Student temp=s[j];
        s[j]=s[j+1];
        s[j+1]=temp;
      }
    }
  }

  printf("Student ID\t\tArrivalTime\t\tDuration\n");
  for(int i=0;i<n;i++){
    printf("%d\t\t%d\t\t%d", s[i].id, s[i].arrival_time, s[i].duration);
    printf("\n");
  }

  pid_t pid=fork();

  if(pid==0){
    printf("Child executing i.e Hostel Allocation System has started");

    sem_init(&room_semaphore, 0, TOTAL_ROOMS);
    pthread_t t[n];

    for(int i=0;i<n;i++){
      pthread_create(&t[i], NULL, allocate, &s[i]);
    }

    for(int i=0;i<n;i++){
      pthread_join(t[i], NULL);
    }

    sem_destroy(&room_semaphore);
    printf("Child Process completed");
    exit(1);
  }else{
    wait(NULL);
    printf("Parent completed i.e Hostel Allocation System closing now!!!");
  }

  return 0;
}