#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



double  a, b, ht,total;
pthread_mutex_t   mutex;
int  thread_size, n, interval;

double f(double x) 
{
  double func;
  func=x*x;
return func;
}

double Trap (double point_a,double point_b,int trap_count,double length)
{
  double value, x;
  value = (f(point_a)+ f(point_b))/2.0;
  
  for (int i = 1; i <= trap_count-1 ; i++)
  {
    x=point_a + (i * length);
    value  +=  f(x);
  }
  
  return  (length*value);

}

void *Trapezoidal(void* rank) {

  long        my_rank = (long) rank;
    double      point_a,point_b,value;    
  
   
    point_a = a + my_rank*interval*ht;
    point_b = point_a + interval*ht;

value=Trap(point_a, point_b, interval, ht);

    pthread_mutex_lock(&mutex);
    total += value;
    pthread_mutex_unlock(&mutex);

    return NULL;
}  



int main(int argc, char** argv) 
{

    pthread_t*  thread_handles;  
 
    thread_size = strtol(argv[1], NULL, 10);
    printf("Enter a, b and n\n");
    scanf("%lf %lf %d", &a, &b, &n);
	
    ht = (b-a)/n;
    interval = n/thread_size;

  thread_handles = malloc (thread_size*sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

   for (long i = 0; i < thread_size; i++) 
   {
         pthread_create(&thread_handles[i], NULL, Trapezoidal,  (void*) i);
    }

    for (long i = 0; i < thread_size; i++) 
    {
        pthread_join(thread_handles[i], NULL);
    }

   
    printf("With n = %d trapezoids, from %f to %f = %19.15e\n", n,a,b,total);


    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    return 0;
} 
