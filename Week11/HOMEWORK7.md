# Homework 7: banker's algorithm
Please write a program to answer the questions in today's quiz using banker's algorithm.

You can input the number of allocated resources of every process, the number of MAX resources of every process, and the total number of resources.

Furthermore, you can input the number of requested resources of process i.

The output is safe state or unsafe state.

### Solution
```C
#include<stdio.h>
#include<stdlib.h>

void print(int [][10], int, int);
void load(int [][10], int [][10], int [][10], int [][10], int*, int*);
int banker(int [][10], int [][10], int [1][10], int, int);
int isSafety(int [][10], int [][10], int [][10], int [], int , int);
void request(int [][10], int [][10], int[][10], int, int);

int main()
{
        int p, r, pid, select;
        // A : Allocation  M : Max  N : Need  W : Wonk
        int A[10][10], M[10][10], N[10][10], W[1][10];

        printf("Homework 7 - Banker's Algorithm\n");
        load(A, M, N, W, &p, &r);

        if(banker(A, N, W, p, r) != 0)
        {
                printf("\nDo you want to add request ? (Yes=1|No=0) ");
                scanf("%d", &select);

                if(select == 1)
                {
                        printf("\n Enter process No. : ");
                        scanf("%d", &pid);
                        request(A, N, W, pid - 1, r);

                        if(banker(A, N, W, p, r) == 0)
                                exit(0);
                }
        }
        else
                exit(0);

        return 0;
}

void load(int A[][10], int M[][10], int N[][10], int W[][10], int *p, int *r)
{
        printf("Enter num of processes : ");
        scanf("%d", p);
        printf("Enter num of resources : ");
        scanf("%d", r);

        for(int i = 0; i < *p; i++)
        {
                printf("\n Process %d\n", i + 1);

                for(int j = 0; j < *r; j++)
                {
                        printf(" Allocation %d : ", j + 1);
                        scanf("%d", &A[i][j]);
                        printf(" Max %d : ", j + 1);
                        scanf("%d", &M[i][j]);
                }
        }

        printf("\n Available :\n");

        for(int i = 0; i < *r; i++)
        {
                printf(" Resource %d : ", i + 1);
                scanf("%d", &W[0][i]);
        }

        for(int i = 0; i < *p; i++)
                for(int j = 0; j < *r; j++)
                        N[i][j] = M[i][j] - A[i][j];

        printf("\n Allocation Matrix\n");
        print(A, *p, *r);
        printf("\n Max Matrix\n");
        print(M, *p, *r);
        printf("\n Need Matrix\n");
        print(N, *p, *r);
}

int banker(int A[][10], int N[][10], int W[1][10], int p, int r)
{
        int avail[10];

        if(isSafety(A, N, W, avail, p, r))
        {
                printf("\nSafety sequence is :\n");

                for(int i = 0; i < p; i++)
                        printf(" P%d  ", avail[i]);

                printf("\n");
                return 1;
        }
        else
        {
                printf("\nDeadlock occured!\n");
                return 0;
        }
}

int isSafety(int A[][10], int N[][10], int W[][10], int avail[], int p, int r)
{
        int F[10], AV[1][10]; 
        int flag = 0, pFlag = 0, count = 0;

        for(int i = 0; i < p; i++)
                F[i] = 0;

        for(int i = 0; i < r; i++)
                AV[0][i] = W[0][i];

        for(int i = 0; i < p; i++)
        {
                for(int j = 0; j < p; j++)
                {
                        if(F[j] == 0)
                        {
                                flag = 0;

                                for(int k = 0; k < r; k++)
                                        if(N[j][k] > AV[0][k])
                                                flag = 1;

                                if((flag == 0)&&(F[j] == 0))
                                {
                                        for(int k = 0; k < r; k++)
                                                AV[0][k] += A[j][k];

                                        F[j] = 1;
                                        pFlag++;
                                        avail[count++] = j;
                                }
                        }
                }

                if(pFlag == p) 
                        return 1;
        }

        return 0;
}

void print(int Mat[][10], int p, int r)
{
        for(int i = 0; i < p; i++)
        {
                for(int j = 0; j < r; j++)
                        printf("%d\t", Mat[i][j]);

                printf("\n");
        }
}

void request(int A[][10], int N[][10], int W[][10], int pid, int p)
{
        int req[1][10];

        printf("\n Enter request :\n");

        for(int i = 0; i < p; i++)
        {
                printf("Request %d : ", i + 1);
                scanf("%d", &req[0][i]);
        }

        for(int i = 0; i < p; i++)
        {
                if(req[0][i] > N[pid][i])
                {
                        printf("\n Error!\n");
                        exit(0);
                }
        }

        for(int i = 0; i < p; i++)
        {
                W[0][i] -= req[0][i];
                A[pid][i] += req[0][i];
                N[pid][i] -= req[0][i];
        }
}

```
[code](banker-s-algorithm.c)
