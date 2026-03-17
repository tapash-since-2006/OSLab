#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 20

int n, quantum;
int at[MAX], bt[MAX], rt[MAX], pid[MAX], priority[MAX];
int wt[MAX], tat[MAX], ct[MAX];

int in_queue[MAX] = {0};

char gantt[1000] = "";
char temp[50];
int last = -2;

// ---------------- CHILD PROCESS: SORT ----------------
void sort_processes() {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if(at[i] > at[j] || (at[i] == at[j] && priority[i] < priority[j])) {
                int tmp;
                tmp = at[i]; at[i] = at[j]; at[j] = tmp;
                tmp = bt[i]; bt[i] = bt[j]; bt[j] = tmp;
                tmp = priority[i]; priority[i] = priority[j]; priority[j] = tmp;
                tmp = pid[i]; pid[i] = pid[j]; pid[j] = tmp;
            }
        }
    }

    printf("\nChild Process: Ready Queue Order (after sorting):\n");
    for (int i = 0; i < n; i++) {
        printf("P%d", pid[i]);
        if (i != n - 1) printf(" -> ");
    }
    printf("\n");
}

// ---------------- PARENT PROCESS: ROUND ROBIN ----------------
void round_robin() {
    for(int i=0;i<n;i++) in_queue[i]=0;
    int queue[1000], front=0, rear=0;
    int time=0, completed=0;

    for(int i=0;i<n;i++) rt[i]=bt[i];

    sprintf(gantt,"%d",time);

    for(int i=0;i<n;i++) {
        if(at[i]<=time){
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    while(completed<n) {
        int idx = (front==rear) ? -1 : queue[front++];
        if(idx != last) {
            if(last!=-2){
                sprintf(temp," | %d",time);
                strcat(gantt,temp);
            }
            if(idx==-1) strcat(gantt," | Idle");
            else{
                sprintf(temp," | P%d",pid[idx]);
                strcat(gantt,temp);
            }
        }

        if(idx==-1){
            time++;
            for(int i=0;i<n;i++){
                if(at[i]<=time && !in_queue[i]){
                    queue[rear++] = i;
                    in_queue[i]=1;
                }
            }
        } else {
            int exec = (rt[idx]>quantum) ? quantum : rt[idx];
            for(int t=0;t<exec;t++){
                time++;
                for(int j=0;j<n;j++){
                    if(at[j]<=time && !in_queue[j]){
                        queue[rear++] = j;
                        in_queue[j]=1;
                    }
                }
            }
            rt[idx]-=exec;
            if(rt[idx]>0) queue[rear++]=idx;
            else {
                completed++;
                ct[idx]=time;
                tat[idx]=ct[idx]-at[idx];
                wt[idx]=tat[idx]-bt[idx];
            }
        }
        last = idx;
    }

    sprintf(temp," | %d",time);
    strcat(gantt,temp);

    printf("\nParent Process: Round Robin Scheduling (Quantum=%d)\n",quantum);
    printf("\nGantt Chart:\n%s\n",gantt);

    float total_tat=0,total_wt=0;
    printf("\n--- Final Scheduling Table ---\n");
    printf("PID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
               pid[i],at[i],bt[i],priority[i],ct[i],tat[i],wt[i]);
        total_tat+=tat[i];
        total_wt+=wt[i];
    }
    printf("\nAverage Turnaround Time = %.2f",total_tat/n);
    printf("\nAverage Waiting Time = %.2f\n",total_wt/n);
}

// ---------------- MAIN ----------------
int main() {
    printf("Number of Processes = ");
    scanf("%d",&n);

    printf("\nPID AT BT Priority\n");
    for(int i=0;i<n;i++){
        scanf("%d %d %d %d",&pid[i],&at[i],&bt[i],&priority[i]);
    }

    quantum=4;

    pid_t pid_child = fork();
    if(pid_child < 0){
        perror("fork failed");
        return 1;
    }

    if(pid_child == 0){
        // Child process sorts the processes
        sort_processes();
        return 0; // child exits
    } else {
        // Parent waits for child to complete sorting
        wait(NULL);
        // Parent does round robin scheduling
        round_robin();
    }

    return 0;
}