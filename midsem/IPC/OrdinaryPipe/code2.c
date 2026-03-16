#include<stdio.h>
#include<unistd.h>

int main()
{
    int p1[2], p2[2];
    int num;

    pipe(p1);
    pipe(p2);

    if(fork()==0)
    {
        // CHILD
        close(p1[1]); // close write
        close(p2[0]); // close read

        read(p1[0], &num, sizeof(num));

        num = num * 2;

        write(p2[1], &num, sizeof(num));
    }
    else
    {
        // PARENT
        close(p1[0]);
        close(p2[1]);

        printf("Enter number: ");
        scanf("%d",&num);

        write(p1[1], &num, sizeof(num));

        read(p2[0], &num, sizeof(num));

        printf("Result from child: %d\n", num);
    }

    return 0;
}