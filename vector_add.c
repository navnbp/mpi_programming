#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>

void Read_n(int* order, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm) {

  int dest;
 if (my_rank == 0)
{
   printf("enter the order of  vector?\n");
   scanf("%d", order);

  for( dest=1;dest<comm_sz;dest++)
  {
    MPI_Send(order,1,MPI_INT,dest,0,MPI_COMM_WORLD); 
  }
      
}
else
{ 
   MPI_Recv(order,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}

 *local_n_p = *order / comm_sz;

 }


void Read_vector(double vec1[], double vec2[], int local_n, int my_rank, int comm_sz, MPI_Comm comm)
 {
   double* temp = NULL;
   int i;
   
   if (my_rank == 0)
   {
      temp = malloc(local_n * comm_sz * sizeof(double));
      printf("Enter the first vector\n");
      for (int i = 0; i < local_n * comm_sz; i++) 
      {
         scanf("%lf", &temp[i]);
       }
       MPI_Scatter(temp, local_n, MPI_DOUBLE, vec1,local_n,MPI_DOUBLE, 0, comm);
       
       printf("Enter the second vector\n");
       for (int i = 0; i < local_n * comm_sz; i++)
        {
               scanf("%lf", &temp[i]);
        } 
        MPI_Scatter(temp, local_n, MPI_DOUBLE, vec2, local_n,MPI_DOUBLE, 0, comm);

      free(temp);
} 
else
 {
      MPI_Scatter(temp, local_n, MPI_DOUBLE, vec1, local_n,MPI_DOUBLE, 0, comm);
       MPI_Scatter(temp, local_n, MPI_DOUBLE, vec2, local_n,MPI_DOUBLE, 0, comm);
  }
} 

void Print_vector(double vec[], int local_n, int n, char title[], int rank, MPI_Comm comm) {
  
   double* temp = NULL;
  int i;
   if (rank == 0) {

      printf("%s\n", title);
      temp = malloc(n * sizeof(double));
      MPI_Gather(vec, local_n, MPI_DOUBLE, temp, local_n,MPI_DOUBLE, 0, comm);
     for ( i = 0; i < n; i++)
     {
       printf("%f ", temp[i]);
      }
   printf("\n");
   free(temp);
   } 
   else {
      MPI_Gather(vec, local_n, MPI_DOUBLE, temp, local_n,MPI_DOUBLE, 0, comm);
   }
   

} 

void Add_vector(double vec1[], double vec2[], double vec_sum[], int n) {
int i;
 for ( i = 0; i < n; i++)
 {
   vec_sum[i] = vec1[i] + vec2[i];
 }
} 


int main(void) {

   int order, local_n;
   double *vec1, *vec2,*vec_sum;
   int comm_sz, my_rank;
   
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   
    Read_n(&order, &local_n, my_rank, comm_sz, MPI_COMM_WORLD);
    vec1 = malloc(sizeof(double)*local_n);
    vec2 = malloc(sizeof(double)*local_n);
  vec_sum = malloc(sizeof(double)*local_n);
   
  Read_vector(vec1, vec2,  local_n, my_rank, comm_sz, MPI_COMM_WORLD); 
     
  Print_vector(vec1, local_n, order, "First Vector :  ", my_rank, MPI_COMM_WORLD);
  Print_vector(vec2, local_n, order, "Second Vector :  ", my_rank, MPI_COMM_WORLD);
   
  Add_vector(vec1, vec2, vec_sum, local_n);
  Print_vector(vec_sum, local_n, order,"Sum of two Vectors is ",my_rank, MPI_COMM_WORLD);
  
   
 
   free(vec_sum);
   free(vec2);
   free(vec1);
   
   MPI_Finalize();
   return 0;
}









