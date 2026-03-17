// #include <stdio.h>

// int main(){
//     int n,i,quantum,remain,time=0;
//     int bt[20],rt[20],wt[20],tat[20];

//     printf("Enter number of processes: ");
//     scanf("%d",&n);

//     printf("Enter burst times:\n");
//     for(i=0;i<n;i++){
//         printf("P%d: ",i+1);
//         scanf("%d",&bt[i]);
//         rt[i]=bt[i];
//     }

//     printf("Enter time quantum: ");
//     scanf("%d",&quantum);

//     remain=n;

//     while(remain>0){
//         for(i=0;i<n;i++){
//             if(rt[i]>0){
//                 if(rt[i]>quantum){
//                     time+=quantum;
//                     rt[i]-=quantum;
//                 }
//                 else{
//                     time+=rt[i];
//                     wt[i]=time-bt[i];
//                     rt[i]=0;
//                     remain--;
//                 }
//             }
//         }
//     }

//     for(i=0;i<n;i++)
//         tat[i]=bt[i]+wt[i];

//     printf("\nProcess\tWT\tTAT\n");
//     for(i=0;i<n;i++)
//         printf("P%d\t%d\t%d\n",i+1,wt[i],tat[i]);
// }




 


// #include <stdio.h>
// #include <string.h>

// int main() {
//     int n, i, quantum;
//     int at[20], bt[20], rt[20];
//     int wt[20], tat[20], ct[20];
//     int pid[20];

//     // --- Added Gantt Chart Variables ---
//     char gantt[1000] = "";
//     char temp[50];
//     int last = -2;   // tracks last running state (-2 initial, -1 idle, >=0 process index)

//     printf("Enter number of processes: ");
//     scanf("%d", &n);

//     printf("\nEnter Arrival Time and Burst Time:\n");
//     for(i = 0; i < n; i++) {
//         pid[i] = i + 1;

//         printf("P%d AT: ", i + 1);
//         scanf("%d", &at[i]);

//         printf("P%d BT: ", i + 1);
//         scanf("%d", &bt[i]);

//         rt[i] = bt[i];
//     }

//     printf("\nEnter Time Quantum: ");
//     scanf("%d", &quantum);

//     int completed=0, time=0;
//     int pointer=0;

//     sprintf(temp,"%d", time);
//     strcat(gantt,temp);

//     while(completed<n){
//         int idx=-1;

//         for(int loop=0;loop<n;loop++){
//             if(at[pointer]<=time && rt[pointer]>0){
//                 idx=pointer;
//                 pointer=(pointer+1)%n;
//             }
//             pointer=(pointer+1)%n;
//         }

//        if(last!=idx){
//         if(last!=-2){
//             sprintf(temp, " | %d", time);
//             strcat(gantt, temp);
//         }

//         if(idx==-1){
//             strcat(gantt, " | Idle");
//         }else{
//             sprintf(temp, " | P%d", pid[idx]);
//             strcat(gantt, temp);
//         }
//        }
       
//        if(idx==-1){
//         time++;
//        }else{
//         if(rt[idx]>quantum){
//             rt[idx]-=quantum;
//             time+=quantum;
//         }else{
//             time+=rt[idx];
//             rt[idx]=0;
//             ct[idx]=time;
//             tat[idx]=ct[idx]-at[idx];
//             wt[idx]=tat[idx]-bt[idx];

//             completed++;
//         }
//        }

//        last=idx;
//     }

//         // Append the final completion time to close out the Gantt chart
//     sprintf(temp, " | %d", time);
//     strcat(gantt, temp);

//     // --- PRINT RESULTS ---
//     printf("\nProcess\tAT\tBT\tWT\tTAT\n");
//     for(i = 0; i < n; i++)
//         printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);

//     printf("\nGantt Chart:\n%s\n", gantt);

//     return 0;

// }


#include <stdio.h>
#include <string.h>

int main() {
    int n, i, quantum;
    int at[20], bt[20], rt[20], pid[20];
    int wt[20], tat[20], ct[20];
    int in_queue[20] = {0}; // Tracks if a process is already in the ready queue

    char gantt[1000] = "";
    char temp[50];
    int last = -2;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and Burst Time:\n");
    for(i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);
        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &quantum);

    // --- Queue Management ---
    int queue[100], front = 0, rear = 0;
    int time = 0, completed = 0;

    sprintf(gantt, "%d", time);

    // Initial check: Add all processes that arrived at time 0
    for(i = 0; i < n; i++) {
        if(at[i] <= time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    while(completed < n) {
        int idx = -1;

        // If queue is empty, CPU is idle
        if(front == rear) {
            idx = -1;
        } else {
            idx = queue[front++]; // Pop the next process from the queue
        }

        // --- Gantt Chart Logic ---
        if(idx != last) {
            if(last != -2) {
                sprintf(temp, " | %d", time);
                strcat(gantt, temp);
            }
            if(idx == -1) {
                strcat(gantt, " | Idle");
            } else {
                sprintf(temp, " | P%d", pid[idx]);
                strcat(gantt, temp);
            }
        }

        // --- Execution Logic ---
        if(idx == -1) {
            time++; 
            // Check if anyone arrives during this idle second
            for(i = 0; i < n; i++) {
                if(at[i] <= time && !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
        } else {
            int execute = (rt[idx] > quantum) ? quantum : rt[idx];
            
            // Run process slice
            for(int t = 0; t < execute; t++) {
                time++;
                // CRITICAL: Check for new arrivals every second during execution
                for(int j = 0; j < n; j++) {
                    if(at[j] <= time && !in_queue[j]) {
                        queue[rear++] = j;
                        in_queue[j] = 1;
                    }
                }
            }
            rt[idx] -= execute;

            if(rt[idx] > 0) {
                // If not finished, it goes back to the END of the queue
                queue[rear++] = idx;
            } else {
                completed++;
                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
            }
        }
        last = idx;
    }

    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}



