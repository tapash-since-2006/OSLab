#include<stdio.h>
#include<pthread.h>

int arr[100];

typedef struct{
  int left;
  int right;
}Range;

void merge(int l, int m, int r)
{
    int i=l, j=m+1, k=0;
    int temp[100];

    while(i<=m && j<=r)
    {
        if(arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while(i<=m)
        temp[k++] = arr[i++];

    while(j<=r)
        temp[k++] = arr[j++];

    for(i=0;i<k;i++)
        arr[l+i] = temp[i];
}

void *merge_sort(void *arg){
  Range *range=(Range*) arg;
  
  int l=range->left;
  int r=range->right;

  if(l>=r){
    return NULL;
  }

  int mid=(l+r)/2;

  Range left={l,mid};
  Range right={mid+1, r};

  pthread_t t1, t2;

  pthread_create(&t1, NULL, merge_sort, &left);
  pthread_create(&t2, NULL, merge_sort, &right);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  merge(l,mid,r);

  return NULL;

}


int main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d",&n);

    printf("Enter elements:\n");

    for(int i=0;i<n;i++)
        scanf("%d",&arr[i]);

    Range full = {0,n-1};

    merge_sort(&full);

    printf("Sorted array:\n");

    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);

    printf("\n");

    return 0;
}