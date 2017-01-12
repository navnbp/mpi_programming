#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int  row, col,total_threads;
double* mat;
double* vec;
double* result;
pthread_mutex_t pthread;

void *matrix_multiplication(void* rank) {
 
   double t;
   long local_rank = (long) rank;
   int  sz = row/total_threads; 
   int z=local_rank*sz*col;
    int start_i = local_rank*sz;
    int end_i = start_i + sz - 1;


   pthread_mutex_lock(&pthread);
   for (int i = start_i; i <= end_i; i++) {
       t = 0.0;
      for (int j = 0; j < col; j++){
          t += mat[z++]*vec[j];
       }
          
       result[i]=t;
   }
      pthread_mutex_unlock(&pthread);
   return NULL;
}  



int main(int argc, char* argv[]) {
   long       th;
   pthread_t* thread_handles;
  
  
   total_threads = strtol(argv[1], NULL, 10);
   thread_handles = malloc(total_threads*sizeof(pthread_t));

   printf("Enter matrix size \n");
   scanf("%d%d", &row, &col);

   vec = malloc(sizeof(double)*col);
   result = malloc(sizeof(double)*row);
    mat = malloc(sizeof(double)*row*col);
   
  
   printf("Enter Matrix Elements\n");
   for (int i = 0; i < row; i++) 
   {
      for (int j = 0; j < col; j++)
      {
        scanf("%lf", &mat[i*col+j]);
      }
   }
        
      printf("Enter Vector Elements\n");
      for (int i = 0; i < col; i++) {
        
      scanf("%lf", &vec[i]);
      }

 printf("Matix Elements\n");
   for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++){
         printf("%2.1f ", mat[i*col + j]);
      }
      printf("\n");
   }
   
 printf("Vector Elements\n");
   for (int i = 0; i < col; i++){
      printf("%2.5f\n", vec[i]);
   }
   printf("\n");


 pthread_mutex_init(&pthread, NULL);  

   for (th = 0; th < total_threads; th++)
   {
      pthread_create(&thread_handles[th], NULL, matrix_multiplication, (void*) th);
   }

   for (th = 0; th < total_threads; th++)
   {
      pthread_join(thread_handles[th], NULL);
   }

   printf("Result\n");
   for (int i = 0; i < col; i++){
      printf("%2.5f\n", result[i]);
   }
   printf("\n");

   free(mat);
   free(vec);
   free(result);
   free(thread_handles);

   return 0;
}  




