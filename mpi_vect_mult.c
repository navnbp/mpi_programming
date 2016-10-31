#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void Check_for_error(int local_ok, char fname[], char message[],MPI_Comm comm);
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,int local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(double local_vec[], int local_n, int n, char title[], int my_rank, MPI_Comm comm);
double Dot_Product(double local_vec1[], double local_vec2[],int local_n, MPI_Comm comm, int my_rank);
void vector_mult_scalar(double local_vec[], double scalar, double local_result[], int local_n);

int main(void) {
   int n, local_n;
   double *local_vec1, *local_vec2;
   double scalar;
   double *local_scalar_mult1, *local_scalar_mult2;
   double dot_product;
   int comm_sz, my_rank;
   MPI_Comm comm= MPI_COMM_WORLD;
   
    MPI_Init(NULL, NULL);

    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);
   

    Read_n(&n, &local_n, my_rank, comm_sz, comm);
    local_vec1 = malloc(sizeof(double)*local_n);
    local_vec2 = malloc(sizeof(double)*local_n);
    local_scalar_mult1 = malloc(sizeof(double)*local_n);
    local_scalar_mult2 = malloc(sizeof(double)*local_n);
   
     Read_data(local_vec1, local_vec2, &scalar, local_n, my_rank, comm_sz, comm); 
     
       Print_vector(local_vec1, local_n, n, "Vector 1 --> ", my_rank, comm);
       Print_vector(local_vec2, local_n, n, "Vector 2 -->", my_rank, comm);
       if (my_rank == 0)
       {
          printf("scalar is %f\n",scalar);
       }

   Dot_Product(local_vec1, local_vec2, local_n, comm,my_rank);
   
     vector_mult_scalar(local_vec1, scalar, local_scalar_mult1, local_n);
    vector_mult_scalar(local_vec2, scalar, local_scalar_mult2, local_n);
    Print_vector(local_scalar_mult1, local_n, n,"first vector * scalar = ",my_rank, comm);
    Print_vector(local_scalar_mult2, local_n, n,"second vector * scalar = ",my_rank ,comm); 
  
   
   free(local_scalar_mult2);
   free(local_scalar_mult1);
   free(local_vec2);
   free(local_vec1);
   
   MPI_Finalize();
   return 0;
}

/*-------------------------------------------------------------------*/
void Check_for_error(
                int       local_num   /* in */, 
                char      fname[]    /* in */,
                char      message[]  /* in */, 
                MPI_Comm  comm       /* in */) {
   int num;
   
   MPI_Allreduce(&local_num, &num, 1, MPI_INT, MPI_MIN, comm);
   if (num == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, 
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}  /* Check_for_error */


/*-------------------------------------------------------------------*/
void Read_n(int* n, int* local_n_p, int my_rank, int comm_sz, 
      MPI_Comm comm) {

int check = 1;

 if (my_rank == 0){
 /* Read n from user */
 printf("enter the order of the vector?\n");
 scanf("%d", n);
 }

 /* Broadcast n*/
 MPI_Bcast(n, 1, MPI_INT, 0, comm);

 /* Error check*/
 if (*n < 0 || *n % comm_sz != 0)
    check =0;
 Check_for_error(check, "Read_n","n should be perfectly divisible by comm_sz", comm);

 /* Calculate n_p*/
 *local_n_p = *n / comm_sz;
}

/*-------------------------------------------------------------------*/
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,int local_n, int my_rank, int comm_sz, MPI_Comm comm) {
   double* array = NULL;
   
   if (my_rank == 0){
      printf("enter is the scalar value?\n");
      scanf("%lf", scalar_p);
   }
   
   MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);
   
   if (my_rank == 0){
      array = malloc(local_n * comm_sz * sizeof(double));
      printf("Enter the first vector\n");
      for (int i = 0; i < local_n * comm_sz; i++) 
         scanf("%lf", &array[i]);

       MPI_Scatter(array, local_n, MPI_DOUBLE, local_vec1,local_n,MPI_DOUBLE, 0, comm);
       printf("Enter the second vector\n");
       for (int i = 0; i < local_n * comm_sz; i++)
        {
               scanf("%lf", &array[i]);
        } 
        MPI_Scatter(array, local_n, MPI_DOUBLE, local_vec2, local_n,MPI_DOUBLE, 0, comm);

      free(array);
} else {
      MPI_Scatter(array, local_n, MPI_DOUBLE, local_vec1, local_n,MPI_DOUBLE, 0, comm);
       MPI_Scatter(array, local_n, MPI_DOUBLE, local_vec2, local_n,MPI_DOUBLE, 0, comm);
  }
}  /* Read_data */


//data  2 4 6 8 10 12 14 16
  //size 4
// processor  0 -- 2 
  //processor 1-- 4
  //processor 2-- 6
  //processor 3-- 8


/*-------------------------------------------------------------------*/
void Print_vector(double local_vec[], int local_n, int n, char title[], int rank, MPI_Comm comm) {
  
   double* array = NULL;
  
   if (rank == 0) {

      printf("%s\n", title);
      array = malloc(n * sizeof(double));
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, comm);
     for (int i = 0; i < n; i++)
     {
       printf("%f ", array[i]);
      }
   printf("\n");
   free(array);
   } else
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, array, local_n,MPI_DOUBLE, 0, comm);
   

}  /* Print_vector */


/*-------------------------------------------------------------------*/
double Dot_Product(double local_vec1[], double local_vec2[], int local_n, MPI_Comm comm, int my_rank) {

 double result, dotProd = 0;
 for (int i = 0; i < local_n; i++){
   dotProd += local_vec1[i] * local_vec2[i];
  }
 
 MPI_Reduce(&dotProd, &result, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

 if (my_rank == 0) 
      printf("Dot Product = %f\n", result);
 return result;
}  /* Dot_Product */


/*-------------------------------------------------------------------*/
void vector_mult_scalar(double local_vec[], double scalar, double local_result[], int n) {

 for (int i = 0; i < n; i++)
 local_result[i] = local_vec[i] * scalar;
}  /* vector_mult_scalar */

















