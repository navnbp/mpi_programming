#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[])
{
   double x,y;
   int i;
   long int count=0,mycount;
   double distance,avg, pi=0.0,PI=3.14159;
   int myid,numprocs,proc;
   MPI_Status status;
   long int myiter=1;
   long int iterval=10000000; 
   long int niter=iterval;
   
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  srand48(time(NULL)*myid);
 
   mycount=0;
   myiter=niter/numprocs;
   for ( i=0; i<myiter; i++) 
   {
      x = (double)drand48();
      y = (double)drand48();
      distance = x*x+y*y;
      if (distance<=1) 
        mycount++; 
   }
    MPI_Reduce(&mycount, &count, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
  
   if (myid ==0) 
   { 
      pi=(double)count/(myiter*numprocs)*4; 
      printf("\n# iters = %ld \t  pi= %2.10f \t error = %2.10f\n", numprocs*myiter,pi,fabs(pi-PI)); 
     
   }
   
   MPI_Finalize();          
  
return 0;
}



