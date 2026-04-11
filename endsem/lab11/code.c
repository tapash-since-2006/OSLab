#include<stdio.h>
#include<stdlib.h>


int abs_val(int x){
  if(x<0) return -x;
  else return x;
}

void fcfs(int req[], int n, int head){
  int thm=0;
  int pos=head;
  printf("\nFCFS: %d", pos);

  for(int i=0;i<n;i++){
    thm+=abs_val(pos-req[i]);
    pos=req[i];
    printf(" -> %d", pos);
  }

  printf("\nTHM = %d\n", thm);
}




void sstf(int req[], int n, int head){
  int visited[n];
  int pos=head;
  int thm=0;

  for(int i=0;i<n;i++){
    visited[i]=0;
  }

  printf("\nSSTF: %d", pos);

  int count=0;
  while(count<n){
    int minDist=99999, idx=-1;

    for(int i=0;i<n;i++){
      if(!visited[i] && abs_val(pos-req[i])<minDist){
        minDist=abs_val(pos-req[i]);
        idx=i;
      }
    }

    visited[idx]=1;
    thm+=minDist;
    pos=req[idx];
    printf(" -> %d", pos);
    count++;
  }
  printf("\nTHM = %d\n", thm); 
}



void scan(int req[], int n, int head, int maxCyl, int dirLeft){
  int sorted[n];
  for(int i=0;i<n;i++) sorted[i]=req[i];
  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-i-1;j++){
      if(sorted[j]>sorted[j+1]){
        int t=sorted[j];
        sorted[j]=sorted[j+1];
        sorted[j+1]=t;
      }
    }
  }

  int thm=0;
  int pos=head;
printf("\nSCAN: %d", pos);
  if(dirLeft){    //go to the left
    for(int i=n-1;i>=0;i--){
      if(sorted[i]<head){
        thm+=abs_val(pos-sorted[i]);
        pos=sorted[i];
        printf(" -> %d",pos);
      }
    }

    // go to the zero
    thm+=abs_val(pos-0);
    pos=0;
    printf(" -> 0");

    // go to the right
    for(int i=0;i<n;i++){
      if(sorted[i]>=head){
        thm+=abs_val(pos-sorted[i]);
        pos=sorted[i];
        printf(" -> %d",pos);
      }
    }
  }else{      // go to the right first
    for(int i=0;i<n;i++){
      if(sorted[i]>=head){
        thm+=abs_val(pos-sorted[i]);
        pos=sorted[i];
        printf(" -> %d",pos);
      }
    }

    // go to the max 
    thm+=maxCyl-pos;
    pos=maxCyl;
    printf(" -> %d", maxCyl);

    // now go to the left
    for(int i=n-1;i>=0;i--){
      if(sorted[i]<head){
        thm+=abs_val(pos-sorted[i]);
        pos=sorted[i];
        printf(" -> %d",pos);
      }
    }
  }
  printf("\nTHM = %d\n", thm);
}


void cscan(int req[], int n, int head, int maxCyl) {
    int sorted[n];
    for (int i = 0; i < n; i++) sorted[i] = req[i];
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (sorted[j] > sorted[j+1]) { int t=sorted[j]; sorted[j]=sorted[j+1]; sorted[j+1]=t; }

    int thm = 0, pos = head;
    printf("\nC-SCAN: %d", pos);

    // go right, service requests >= head
    for (int i = 0; i < n; i++)
        if (sorted[i] >= head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }

    // go to maxCyl (count this movement)
    thm += maxCyl - pos; pos = maxCyl; printf(" -> %d", maxCyl);

    // jump to 0 — NOT counted in THM
    printf(" -> [jump] -> 0"); pos = 0;

    // service remaining requests from left
    for (int i = 0; i < n; i++)
        if (sorted[i] < head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }

    printf("\nTHM = %d\n", thm);
}




void look(int req[], int n, int head, int dirLeft) {
    int sorted[n];
    for (int i = 0; i < n; i++) sorted[i] = req[i];
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (sorted[j] > sorted[j+1]) { int t=sorted[j]; sorted[j]=sorted[j+1]; sorted[j+1]=t; }

    int thm = 0, pos = head;
    printf("\nLOOK: %d", pos);

    if (dirLeft) {
        // go left only as far as smallest request
        for (int i = n-1; i >= 0; i--)
            if (sorted[i] < head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }
        // reverse and go right
        for (int i = 0; i < n; i++)
            if (sorted[i] >= head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }
    } else {
        for (int i = 0; i < n; i++)
            if (sorted[i] >= head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }
        for (int i = n-1; i >= 0; i--)
            if (sorted[i] < head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }
    }
    printf("\nTHM = %d\n", thm);
}



void clook(int req[], int n, int head) {
    int sorted[n];
    for (int i = 0; i < n; i++) sorted[i] = req[i];
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (sorted[j] > sorted[j+1]) { int t=sorted[j]; sorted[j]=sorted[j+1]; sorted[j+1]=t; }

    int thm = 0, pos = head;
    printf("\nC-LOOK: %d", pos);

    // go right, service requests >= head
    for (int i = 0; i < n; i++)
        if (sorted[i] >= head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }

    // jump to smallest remaining — NOT counted
    int hasLeft = 0;
    for (int i = 0; i < n; i++) if (sorted[i] < head) { hasLeft=1; break; }
    if (hasLeft) {
        printf(" -> [jump] -> %d", sorted[0]); pos = sorted[0];
        // service requests < head going upward
        for (int i = 1; i < n; i++)
            if (sorted[i] < head) { thm += abs_val(pos-sorted[i]); pos=sorted[i]; printf(" -> %d",pos); }
    }
    printf("\nTHM = %d\n", thm);
}


int main() {
    int n, head, maxCyl;
    printf("Number of requests: "); scanf("%d", &n);
    int req[n];
    printf("Enter requests: ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Current head position: "); scanf("%d", &head);
    printf("Max cylinders: "); scanf("%d", &maxCyl);

    fcfs(req, n, head);
    sstf(req, n, head);
    scan(req, n, head, maxCyl-1, 1);   // 1 = go left first
    cscan(req, n, head, maxCyl-1);
    look(req, n, head, 1);
    clook(req, n, head);

    return 0;
}