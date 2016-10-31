#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 


int main(int argc, char** argv){


void Read_vector(double vector1[], double vector2[], int local_n, int rank, int size, MPI_Datatype type);
void Print_vector(double vector[], int local_n, int n, char title[], int rank, MPI_Datatype type);
void Add_vector(double vector1[], double vector2[], double local_result[], int local_n);


   int n,size, rank, local_n;
   double *vector1, *vector2, *result;
  
   
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    

    if (rank == 0)
    {
     printf("Enter the order of the vector?\n");
     scanf("%d", &n);
    }

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n = n / size;


     MPI_Datatype type;
    MPI_Type_contiguous(local_n,MPI_DOUBLE,&type);
    MPI_Type_commit(&type);

    //Read_order(&n, &local_n, rank, size);
    vector1 = malloc(sizeof(double)*local_n);
    vector2 = malloc(sizeof(double)*local_n);
    result = malloc(sizeof(double)*local_n);

   
    Read_vector(vector1, vector2,  local_n, rank, size,type); 
     
    Print_vector(vector1, local_n, n, "Vector 1 --> ", rank,type);
    Print_vector(vector2, local_n, n, "Vector 2 -->", rank,type);
      
   
    Add_vector(vector1, vector2, result, local_n);
    Print_vector(result, local_n, n,"Vector Sum --> ",rank,type);

   free(result);
   free(vector2);
   free(vector1);
   
   MPI_Finalize();
   return 0;
}


// void Read_order(int* n, int* local_n, int rank, int size) {


//  if (rank == 0){
//  printf("enter the order of the vector?\n");
//  scanf("%d", n);
//  }

 
//  MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
//  *local_n = *n / size;
// }


void Read_vector(double vector1[], double vector2[], int local_n, int rank, int size, MPI_Datatype type)
 {
   double* array = NULL;
   
   if (rank == 0)
   {
      array = malloc(local_n * size * sizeof(double));
      printf("Enter the first vector\n");
      for (int i = 0; i < local_n * size; i++) 
         scanf("%lf", &array[i]);
       MPI_Scatter(array, 1, type, vector1,1,type, 0, MPI_COMM_WORLD);
      
       printf("Enter the second vector\n");
       for (int i = 0; i < local_n * size; i++)
            scanf("%lf", &array[i]);
        MPI_Scatter(array, 1, type, vector2, 1,type, 0, MPI_COMM_WORLD);

      free(array);
} else {
      MPI_Scatter(array, 1, type, vector1, 1,type, 0, MPI_COMM_WORLD);
       MPI_Scatter(array, 1, type, vector2, 1,type, 0, MPI_COMM_WORLD);
  }
} 

void Print_vector(double vector[], int local_n, int n, char title[], int rank, MPI_Datatype type) {
  
   double* array = NULL;
  
   if (rank == 0) {

      printf("%s\n", title);
      array = malloc(n * sizeof(double));
      MPI_Gather(vector, 1, type, array, 1,type, 0, MPI_COMM_WORLD);
     for (int i = 0; i < n; i++)
     {
       printf("%f ", array[i]);
      }
   printf("\n");
   free(array);
   } else
      MPI_Gather(vector, 1, type, array, 1,type, 0, MPI_COMM_WORLD);
   

} 



void Add_vector(double vector1[], double vector2[], double result[], int n) 
{

 for (int i = 0; i < n; i++)
 result[i] = vector1[i] + vector2[i];
} 



// proc -> 3 array size =9
// type -> MPI_DOUBLE*3
// 3 -> sizeof(MPI_DOUBLE)*1



