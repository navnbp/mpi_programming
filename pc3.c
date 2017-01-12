
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count, message_avaliable;  
pthread_mutex_t mutex;

void Usage(char* prog_name);
void *Hello(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;

   /* Pega o número de threads da linha de comando */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]); 
   
   message_avaliable = 0;

   thread_handles = malloc (thread_count*sizeof(pthread_t));

   pthread_mutex_init(&mutex, NULL); 

   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Hello, (void*) thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   free(thread_handles);
   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *Hello(void* rank) {
   long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 


	pthread_mutex_lock(&mutex);
	
		if (message_avaliable) {
			printf("Consumer %li i\n",my_rank);
			message_avaliable = 0;
			pthread_mutex_unlock(&mutex);
			
		}
	else { //Produtor é par
		printf("Producer %li \n",my_rank);
		message_avaliable = 1;
		pthread_mutex_unlock(&mutex);
		
	}

 

   return NULL;
}  /* Hello */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
