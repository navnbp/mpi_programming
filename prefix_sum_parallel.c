#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 


int main(int argc, char** argv){


void Read_vector(double vector1[], int local_n, int rank, int size);
void Print_vector(double vector[], int local_n, int n, char title[], int rank,int size);
double Add_vector(double vector1[], double total, double result[], int n,int rank,int size) ;
void Print_Result(double vector[], int local_n, int n, char title[], int rank,int size);

  int n,size, rank, local_n;
  double *vector1,  *result,ksum;
  double total;
   
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if (rank == 0)
 {
     printf("Enter the order of the vector?\n");
     scanf("%d", &n);
}

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
 //  MPI_Bcast(&total, 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
    local_n = n / size;

   vector1 = malloc(sizeof(double)*local_n);
   result = malloc(sizeof(double)*local_n);


    Read_vector(vector1,  local_n, rank, size); 

    Print_vector(vector1, local_n, n, "Array : --> ", rank,size);

    // MPI_Reduce(&vector1, &ksum, local_n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    //   if (rank == 0) {
    //     printf("Reduce: %d\n", ksum);
    //   }

 //MPI_Barrier(MPI_COMM_WORLD);
  // MPI_Allreduce(&vector1, &ksum, local_n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  // printf("%d: Allreduce: %d\n", rank, ksum);
   // MPI_Barrier(MPI_COMM_WORLD);
  // MPI_Recv(&total,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
     // printf("start  rank --> %d total -->%lf\n",rank,total );
    total = Add_vector(vector1, total, result, local_n,rank,size);
    // printf("rank --> %d total -->%lf\n",rank,total );
     // MPI_Barrier(MPI_COMM_WORLD);
    Print_Result(result, local_n, n,"Prefix Sum --> ",rank,size);

   free(result);
   free(vector1);
   
   MPI_Finalize();
   return 0;
}



void Read_vector(double vector1[], int local_n, int rank, int size)
 {
   double* array = NULL;
   
   if (rank == 0)
   {
      array = malloc(local_n * size * sizeof(double));
      printf("Enter the first vector\n");
      for (int i = 0; i < local_n * size; i++) 
         scanf("%lf", &array[i]);
       MPI_Scatter(array, local_n, MPI_DOUBLE, vector1,local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      
      free(array);
} else {
      MPI_Scatter(array, local_n, MPI_DOUBLE, vector1, local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);
     }
} 



void Print_Result(double vector[], int local_n, int n, char title[], int rank,int size) {
  
  int temp,i;  
   double* array = NULL;
   double* sum = NULL;
    double total=0; 
   if (rank == 0) {

      printf("%s\n", title);
      array = malloc(n * sizeof(double));
      sum = malloc(n * sizeof(double));
        temp= n/size;
      MPI_Gather(vector, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for ( i = 0; i < n; i++)
        sum[i]=array[i];
     
  for ( i = 0; i < n; i++)
  {
        
        sum[i]=i<temp?array[i]:total+array[i];
        
        if(i%temp==temp-1)
          total=sum[i];

      printf("%f ", sum[i]);   
       }

    
   printf("\n");
   free(array);
   } else{
         MPI_Gather(vector, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);


      }
  }

void Print_vector(double vector[], int local_n, int n, char title[], int rank,int size) {
  
   double* array = NULL;
  
   if (rank == 0) {

      printf("%s\n", title);
      array = malloc(n * sizeof(double));
      MPI_Gather(vector, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for (int i = 0; i < n; i++)
     {
       printf("%f ", array[i]);
      }
   printf("\n");
   free(array);
   } else{
         MPI_Gather(vector, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, MPI_COMM_WORLD);


      }
  } 



double Add_vector(double vector1[], double total, double result[], int n,int rank, int size) 
{
   // printf("Total --> %f Rank --> %d \n",total,rank );
    for (int i = 0; i < n; i++)
    {
        total +=vector1[i];
         result[i] = i==0?total:result[i-1]+vector1[i];
    }
   MPI_Bcast(&total, 1, MPI_DOUBLE, 1, MPI_COMM_WORLD);
     return total;
} 

