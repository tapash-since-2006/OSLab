#include<stdio.h>
#define MAX 10
int n,m;

int isDeadlock(int avail[],int request[MAX][MAX],int allot[MAX][MAX]){

int work[MAX];
int finish[MAX];
int j;

for(int i=0;i<m;i++)
work[i]=avail[i];

for(int i=0;i<n;i++){
finish[i]=1;
for(int j=0;j<m;j++){
if(allot[i][j]!=0){
finish[i]=0;
break;
}
}
}

int count=0;
while(count<n){
int found=0;
for(int i=0;i<n;i++){
if(!finish[i]){
for(j=0;j<m;j++){
if(request[i][j]>work[j]){
break;
}
}
if(j==m){
for(int k=0;k<m;k++){
work[k]+=allot[i][k];
}
finish[i]=1;
found=1;
count++;
}
}
}
if(!found){
break;
}
}

int deadlock=0;
for(int i=0;i<n;i++){
if(!finish[i]){
deadlock=1;
break;
}
}

if(deadlock){
printf("\nDeadlock detected!\n");
printf("Processes in deadlock: ");
for(int i=0;i<n;i++){
if(!finish[i])
printf("P%d ",i);
}
printf("\n");
return 1;
}else{
printf("\nNo deadlock. System is in a safe state.\n");
return 0;
}

}

int main(){

int avail[MAX],request[MAX][MAX],allot[MAX][MAX];

printf("Enter the number of processes: ");
scanf("%d",&n);

printf("Enter the number of resource types: ");
scanf("%d",&m);

printf("\nEnter Allocation Matrix:\n");
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
scanf("%d",&allot[i][j]);

printf("\nEnter Request Matrix:\n");
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
scanf("%d",&request[i][j]);

printf("\nEnter Available Resources:\n");
for(int i=0;i<m;i++)
scanf("%d",&avail[i]);

printf("\n(a)");
isDeadlock(avail,request,allot);

printf("\n(b) P2 makes additional request for 1 instance of C:\n");
request[2][2]+=1;
isDeadlock(avail,request,allot);

return 0;

}

