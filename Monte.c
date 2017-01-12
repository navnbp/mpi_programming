#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

 
 int niter = 100000,i, count=0,total_threads;                      
  
pthread_mutex_t pthread;

void *calculatePiValue(void* rank) {
   long my_rank = (long) rank;
   double x,y,z;   
   int sz = niter/total_threads; 
   int start_i = my_rank*sz;
   int end_i = start_i + sz - 1;

     pthread_mutex_lock(&pthread);
 for (i=start_i; i<end_i; ++i)                
        {
          
            x = (double)random()/RAND_MAX;     
            y = (double)random()/RAND_MAX;     
            z = sqrt((x*x)+(y*y));             
            if (z<=1)
            {
                ++count;                
            }

           
        }
       pthread_mutex_unlock(&pthread);
        
     
   return NULL;
}  



int main(int argc, char* argv[])
{
    long       th;
     pthread_t* thread_handles;


    printf("Enter the value of N\n");
   scanf("%d", &niter);

 
   total_threads = strtol(argv[1], NULL, 10);
   pthread_mutex_init(&pthread, NULL);  
   thread_handles = malloc(sizeof(pthread_t)*total_threads);
  

     for (th = 0; th < total_threads; th++)
      pthread_create(&thread_handles[th], NULL, calculatePiValue, (void*) th);

   for (th = 0; th < total_threads; th++)
      pthread_join(thread_handles[th], NULL);
        
 
        double pi = ((double)count/(double)niter)*4.0;             
        printf("Pi: %f\nNumber of Points inside circle %i\n", pi, count);
     
 
 
                  
    return 0;
}