#include<stdio.h>
#define MAX 10

void firstFit(int partitions[],int psize,int processes[],int prsize){

int alloc[MAX];
int remain[MAX];

for(int i=0;i<psize;i++){
remain[i]=partitions[i];
alloc[i]=-1;
}

printf("\nFirst Fit:\n");
printf("Process\t\tSize\t\tPartition\n");

for(int i=0;i<prsize;i++){
int allocated=0;
for(int j=0;j<psize;j++){
if(remain[j]>=processes[i]){
alloc[i]=j+1;
remain[j]-=processes[i];
allocated=1;
break;
}
}
if(allocated)
printf("P%d\t\t%dK\t\tP%d(%dK remaining)\n",i+1,processes[i],alloc[i],remain[alloc[i]-1]);
else
printf("P%d\t\t%dK\t\tNot Allocated\n",i+1,processes[i]);
}
}

void bestFit(int partitions[],int psize,int processes[],int prsize){

int alloc[MAX];
int remain[MAX];

for(int i=0;i<psize;i++){
remain[i]=partitions[i];
alloc[i]=-1;
}

printf("\nBest Fit:\n");
printf("Process\t\tSize\t\tPartition\n");

for(int i=0;i<prsize;i++){
int bestIdx=-1;
for(int j=0;j<psize;j++){
if(remain[j]>=processes[i]){
if(bestIdx==-1||remain[j]<remain[bestIdx]){
bestIdx=j;
}
}
}
if(bestIdx!=-1){
alloc[i]=bestIdx+1;
remain[bestIdx]-=processes[i];
printf("P%d\t\t%dK\t\tP%d(%dK remaining)\n",i+1,processes[i],alloc[i],remain[bestIdx]);
}else{
printf("P%d\t\t%dK\t\tNot Allocated\n",i+1,processes[i]);
}
}
}

void worstFit(int partitions[],int psize,int processes[],int prsize){

int alloc[MAX];
int remain[MAX];

for(int i=0;i<psize;i++){
remain[i]=partitions[i];
alloc[i]=-1;
}

printf("\nWorst Fit:\n");
printf("Process\t\tSize\t\tPartition\n");

for(int i=0;i<prsize;i++){
int worstIdx=-1;
for(int j=0;j<psize;j++){
if(remain[j]>=processes[i]){
if(worstIdx==-1||remain[j]>remain[worstIdx]){
worstIdx=j;
}
}
}
if(worstIdx!=-1){
alloc[i]=worstIdx+1;
remain[worstIdx]-=processes[i];
printf("P%d\t\t%dK\t\tP%d(%dK remaining)\n",i+1,processes[i],alloc[i],remain[worstIdx]);
}else{
printf("P%d\t\t%dK\t\tNot Allocated\n",i+1,processes[i]);
}
}
}

int main(){

int partitions[]={100,500,200,300,600};
int processes[]={212,417,112,426};
int psize=5;
int prsize=4;

printf("Memory Partitions: 100K 500K 200K 300K 600K");
printf("\nProcesses: 212K 417K 112K 426K\n");

firstFit(partitions,psize,processes,prsize);

bestFit(partitions,psize,processes,prsize);

worstFit(partitions,psize,processes,prsize);

printf("\nBest Fit is most efficient as it allocates all processes with least wasted memory.\n");

return 0;

}

