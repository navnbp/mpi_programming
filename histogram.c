#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

 int       bin_ct,i=0,data_count,input_Array_size,my_rank,comm_sz,bin,total_threads;
  int*      counts;
  float     minimum,interval,maximum;
  float*    input_Array;
pthread_mutex_t pthread;
  float*    boundries; 


void Read_Data( )
 {
  
    printf("Number of elements for ploting graph  \n");
    scanf("%d",&data_count);
    
    printf("Number of bins ( Intervals)  \n");
    scanf("%d",&bin_ct);
    
    printf("Minimum value / Starting value  \n");
    scanf("%f",&minimum);
    
    printf("Maximum value / End Value \n");
    scanf("%f",&maximum);
  
} 

void Assign_Random_Number()
{
  
  int i;
  float range = maximum - minimum;
  
    input_Array = malloc(data_count*sizeof(float));

    printf("Random Generated Array :\n");
    for(i=0;i<data_count;i++) {
      input_Array[i] = (float) rand() / (float) RAND_MAX * range + minimum;
    printf("%f\n",input_Array[i] );
    }

} 


void display() {

  int indx, k;

  for(indx = 0; indx< bin_ct; indx++) 
  {
    printf("%15.3f \t ",boundries[indx]);
    printf("  %d\n",counts[indx]);
  }
} 

void *calculate(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int sz = data_count/total_threads; 
   int start_i = my_rank*sz;
   int end_i = start_i + sz - 1;
 pthread_mutex_lock(&pthread);
for(i = start_i; i <= end_i; i++) 
{
   
     for(bin = 0; bin < bin_ct; bin++) 
    {
       if(input_Array[i] <= boundries[bin]){
         break;
       }
    }
    counts[bin]=counts[bin]+1;
  }
    pthread_mutex_unlock(&pthread);

   return NULL;
}  

int main(int argc, char* argv[]) {

  long       th;
   pthread_t* thread_handles;

  
   total_threads = strtol(argv[1], NULL, 10);
   thread_handles = malloc(total_threads*sizeof(pthread_t));
 
  Read_Data();


  boundries = malloc(sizeof(float)*bin_ct);
  // bins = malloc(sizeof(int)*bin_ct);
  counts = malloc(sizeof(int)*bin_ct);
  

 for(i = 0; i < bin_ct; i++) {
    interval=(float)( (float)(maximum-minimum) / bin_ct);
    boundries[i] = (float)( interval * (float)(i+1)) +(float) minimum;
    counts[i] = 0;
  }

  Assign_Random_Number();

   pthread_mutex_init(&pthread, NULL);  

 for (th = 0; th < total_threads; th++)
      pthread_create(&thread_handles[th], NULL, calculate, (void*) th);

   for (th = 0; th < total_threads; th++)
      pthread_join(thread_handles[th], NULL);

  
    display();
  
  free(boundries);free(counts);
  free(input_Array);


  return 0;
}


