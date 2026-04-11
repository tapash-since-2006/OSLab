#include <stdio.h>
#include <stdbool.h>

#define mp 10
#define mr 10

int n, m;
int available[mr];
int max[mp][mr];
int alloc[mp][mr];
int need[mp][mr];

void calculateNeed(){
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      need[i][j]=max[i][j]-alloc[i][j];
    }
  }
}

bool isSafe(int safeSeq[]){
  int work[mr];
  bool finish[mp]={false};

  for(int j=0;j<m;j++){
    work[j]=available[j];
  }

  int count=0;
  while(count<n){
    bool found=false;
    for(int i=0;i<n;i++){
      if(!finish[i]){
        bool canAllocate=true;

        for(int j=0;j<m;j++){
          if(need[i][j]>work[j]){
            canAllocate=false;
            break;
          }
        }
        if(canAllocate){
          for(int j=0;j<m;j++){
            work[j]+=alloc[i][j];
          }
          finish[i]=true;
          safeSeq[count++]=i;
          found=true;
        }
      }
    }
    if(!found){
      return false;
    }
  }
  return true;
}


bool requestResource(int pid, int request[]){
  for(int j=0;j<m;j++){
    if(request[j]>need[pid][j]){
      printf("Error: Process exceeds maximum claim\n");
      return false;
    }
  }

  for(int j=0;j<m;j++){
    if(request[j]>available[j]){
      printf("Process P%d must wait. Resource not available\n", pid);
      return false;
    }
  }

  for(int j=0;j<m;j++){
    available[j]-=request[j];
    alloc[pid][j]+=request[j];
    need[pid][j]-=request[j];
  }

  int safeSeq[mp];
  if(isSafe(safeSeq)){
    printf("Request Granted. Sade Sequence: ");
    for(int i=0;i<n;i++){
      printf("P%d ", safeSeq[i]);
    }
    printf("\n");
    return true;
  }else{
    for(int j=0;j<m;j++){
      available[j]+=request[j];
      alloc[pid][j]-=request[j];
      need[pid][j]+=request[j];
    }

    printf("Request denied. Would lead to unsafe state. \n");
    return false;
  }
}

int main(){
  printf("Enter the number of the processes: ");
  scanf("%d", &n);
  printf("Enter the number of resource types: ");
  scanf("%d", &m);

  printf("Enter Available resources:\n");
    for (int j = 0; j < m; j++) scanf("%d", &available[j]);

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    calculateNeed();

    printf("\n--- Need Matrix ---\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    int safeSeq[mp];
    if (isSafe(safeSeq)) {
        printf("\nSystem is in SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE state!\n");
    }

    int pid, request[mr];
    printf("\nEnter process number making a request: ");
    scanf("%d", &pid);
    printf("Enter request vector (%d values): ", m);
    for (int j = 0; j < m; j++) scanf("%d", &request[j]);

    requestResource(pid, request);

    return 0;
}