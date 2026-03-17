#include <stdio.h>
#include <math.h>
#include <string.h>

int gcd(int a, int b){
    while(b != 0){
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b){
    if (a == 0 || b == 0) return 0;
    return (a * b) / gcd(a, b);
}

int main(){

    int n, i, time = 0;
    int exec[10], period[10], rt[10], deadline[10];
    int hyper;
    float utilization = 0;

    char gantt[1000] = "";
    char temp[50];
    int last = -2;

    printf("Enter number of tasks: ");
    scanf("%d", &n);

    printf("\nEnter Execution Time and Period\n");
    for(i = 0; i < n; i++){
        printf("Task %d Execution Time: ", i + 1);
        scanf("%d", &exec[i]);

        printf("Task %d Period: ", i + 1);
        scanf("%d", &period[i]);

        rt[i] = 0;
        deadline[i] = 0;
        utilization += (float)exec[i] / period[i];
    }

    hyper = period[0];
    for(i = 1; i < n; i++)
        hyper = lcm(hyper, period[i]);

    printf("\nHyperperiod = %d\n", hyper);
    printf("Total Utilization = %.3f\n", utilization);

    if(utilization > 1.0)
        printf("Warning: Utilization > 1. System is NOT schedulable.\n");
    else
        printf("System is schedulable (Utilization <= 1).\n");

    sprintf(gantt, "%d", time);

    while(time < hyper){

        for(i = 0; i < n; i++){
            if(time % period[i] == 0){
                if(rt[i] > 0 && time > 0) {
                    printf("\n[!] DEADLINE MISS: Task T%d missed at time %d\n", i + 1, time);
                }
                rt[i] = exec[i];
                deadline[i] = time + period[i]; 
            }
        }

        int idx = -1;
        int earliest = 99999; 

        for(i = 0; i < n; i++){
            if(rt[i] > 0 && deadline[i] < earliest){
                earliest = deadline[i];
                idx = i;
            }
        }

        if(idx != last) {
            if(last != -2) {
                sprintf(temp, " | %d", time);
                strcat(gantt, temp);
            }
            if(idx == -1) {
                strcat(gantt, " | Idle");
            } else {
                sprintf(temp, " | T%d", idx + 1);
                strcat(gantt, temp);
            }
        }

        if(idx == -1){
            time++; 
        }
        else{
            rt[idx]--; 
            time++;
        }
        
        last = idx; 
    }

    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}