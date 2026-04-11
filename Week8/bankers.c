#include<stdio.h>
#define MAX 10
int n,m;

void needMatrix(int need[MAX][MAX],int max[MAX][MAX],int allot[MAX][MAX]){
for(int i=0;i<n;i++){
for(int j=0;j<m;j++){
need[i][j]=max[i][j]-allot[i][j];
}
}
}

int isSafe(int avail[],int max[MAX][MAX],int allot[MAX][MAX]){
int need[MAX][MAX];
needMatrix(need,max,allot);
int finish[MAX]={0};
int safeSeq[MAX];
int work[MAX];
int j;
for(int i=0;i<m;i++){
work[i]=avail[i];
}
int count=0;
while(count<n){
int found=0;
for(int i=0;i<n;i++){
if(!finish[i]){
for(j=0;j<m;j++){
if(need[i][j]>work[j]){
break;
}
}
if(j==m){
for(int k=0;k<m;k++){
work[k]+=allot[i][k];
}
safeSeq[count++]=i;
finish[i]=1;
found=1;
}
}
}
if(!found){
printf("\nSystem is NOT in a safe state.\n");
return 0;
}
}
printf("\nSystem is in a safe state.\n");
printf("The safe sequence is: ");
for(int i=0;i<n;i++){
printf("P%d",safeSeq[i]);
if(i<n-1)printf("->");
}
printf("\n");
return 1;
}

void requestResources(int process,int request[],int avail[],int max[MAX][MAX],int allot[MAX][MAX]){
int need[MAX][MAX];
needMatrix(need,max,allot);
for(int i=0;i<m;i++){
if(request[i]>need[process][i]){
printf("\nRequest exceeds the need.\n");
return;
}
}
for(int i=0;i<m;i++){
if(request[i]>avail[i]){
printf("\nRequest exceeds the available number of resources.\n");
return;
}
}
for(int i=0;i<m;i++){
avail[i]-=request[i];
allot[process][i]+=request[i];
}
if(isSafe(avail,max,allot)){
printf("Request granted.\n");
}else{
printf("Request not granted.(unsafe state)\n");
for(int i=0;i<m;i++){
avail[i]+=request[i];
allot[process][i]-=request[i];
}
}
}

int main(){

int avail[MAX],max[MAX][MAX],allot[MAX][MAX],need[MAX][MAX];
int avail_orig[MAX],allot_orig[MAX][MAX];
int process,request[MAX];

printf("Enter the number of processes: ");
scanf("%d",&n);

printf("Enter the number of resource types: ");
scanf("%d",&m);

printf("\nEnter Allocation Matrix:\n");
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
scanf("%d",&allot[i][j]);

printf("\nEnter Max Matrix:\n");
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
scanf("%d",&max[i][j]);

printf("\nEnter Available Resources:\n");
for(int i=0;i<m;i++)
scanf("%d",&avail[i]);

for(int i=0;i<m;i++)
avail_orig[i]=avail[i];
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
allot_orig[i][j]=allot[i][j];

printf("\n(a) Need Matrix:\n");
needMatrix(need,max,allot);
for(int i=0;i<n;i++){
printf("P%d: ",i);
for(int j=0;j<m;j++)
printf("%d ",need[i][j]);
printf("\n");
}

printf("\n(b)");
isSafe(avail,max,allot);

for(int i=0;i<m;i++)
avail[i]=avail_orig[i];
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
allot[i][j]=allot_orig[i][j];

printf("\n(c) P1 requests (1,0,2):\n");
process=1;
request[0]=1;
request[1]=0;
request[2]=2;
requestResources(process,request,avail,max,allot);

for(int i=0;i<m;i++)
avail[i]=avail_orig[i];
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
allot[i][j]=allot_orig[i][j];

printf("\n(d) P4 requests (3,3,0):\n");
process=4;
request[0]=3;
request[1]=3;
request[2]=0;
requestResources(process,request,avail,max,allot);

for(int i=0;i<m;i++)
avail[i]=avail_orig[i];
for(int i=0;i<n;i++)
for(int j=0;j<m;j++)
allot[i][j]=allot_orig[i][j];

printf("\n(e) P0 requests (0,2,0):\n");
process=0;
request[0]=0;
request[1]=2;
request[2]=0;
requestResources(process,request,avail,max,allot);

return 0;

}
