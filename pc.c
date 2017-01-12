#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

int thread_count,producer_consumer;  
pthread_mutex_t mutex;

void* allotResource(void* rank) {
   long my_rank = (long) rank; 

  
  pthread_mutex_lock(&mutex);
        if(producer_consumer == 0){
           printf("Producer %li \n",my_rank);
           producer_consumer = 1;
          }else{
           printf("Consumer %li \n",my_rank);
           producer_consumer = 0;
           }
 pthread_mutex_unlock(&mutex);
   return NULL;
} 

int main(int argc, char* argv[]) {
   long       thread; 
   pthread_t* thread_handles;

   thread_count = strtol(argv[1], NULL, 10);  
   
   producer_consumer = 0;

   thread_handles = malloc (thread_count*sizeof(pthread_t));

   pthread_mutex_init(&mutex, NULL); 

   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL, allotResource, (void*) thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   free(thread_handles);
   return 0;
}  


