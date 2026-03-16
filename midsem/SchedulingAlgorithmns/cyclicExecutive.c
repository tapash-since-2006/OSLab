#include <stdio.h>

int main(){

    int n,i,time=0,completed=0;
    int at[20], et[20], rt[20], deadline[20];
    int ct[20], wt[20], tat[20];

    printf("Enter number of tasks: ");
    scanf("%d",&n);

    printf("\nEnter Arrival Time, Execution Time and Deadline\n");

    for(i=0;i<n;i++){

        printf("Task %d AT: ",i+1);
        scanf("%d",&at[i]);

        printf("Task %d ET: ",i+1);
        scanf("%d",&et[i]);

        printf("Task %d Deadline: ",i+1);
        scanf("%d",&deadline[i]);

        rt[i]=et[i];
    }

    printf("\nExecution Order:\n");

    while(completed<n){

        int executed=0;

        for(i=0;i<n;i++){

            if(at[i]<=time && rt[i]>0){

                printf("T%d ",i+1);

                time+=rt[i];
                rt[i]=0;

                ct[i]=time;
                completed++;

                executed=1;
            }
        }

        if(executed==0)
            time++;
    }

    for(i=0;i<n;i++){
        tat[i]=ct[i]-at[i];
        wt[i]=tat[i]-et[i];
    }

    printf("\n\nTask\tAT\tET\tDeadline\tCT\tWT\tTAT\tStatus\n");

    for(i=0;i<n;i++){

        if(ct[i] <= deadline[i])
            printf("T%d\t%d\t%d\t%d\t\t%d\t%d\t%d\tMet\n",
            i+1,at[i],et[i],deadline[i],ct[i],wt[i],tat[i]);
        else
            printf("T%d\t%d\t%d\t%d\t\t%d\t%d\t%d\tMissed\n",
            i+1,at[i],et[i],deadline[i],ct[i],wt[i],tat[i]);
    }

    return 0;
}