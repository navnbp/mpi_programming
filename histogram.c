#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main (int argc, char* argv[])
{
	srand(time(NULL));
	int rank,size,n,bin_count,min,max,i,index,result[50],max_elem=0,destination;
	double randomArray[100], val,range,deci;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

if (rank == 0) {
       printf("Enter Number of elements , Bucket Size, Minimum value, Maximum value \n");
		scanf("%d",&n);	
		scanf("%d",&bin_count);	
		scanf("%d",&min);
		scanf("%d",&max);	
		  for ( i = 0; i < n; i++)
		  {
	
		  	 randomArray[i]= (rand() % ((max-min) + min)) ;
		  	  deci= fmod(rand(),0.87);
		  	 randomArray[i] += deci-floor(deci);
		  }
		  
		 
		  for ( i = 0; i < 50; i++)
		  {
		  	result[i]=0;
		  }
			range=(double)(max-min)/bin_count;	
		

     for (destination = 1; destination < comm_sz; destination++) 
		{
	         MPI_Send(&n, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
	         MPI_Send(&bin_count, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
	         MPI_Send(&min, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
	          MPI_Send(&max, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
	            MPI_Send(&randomArray, 100, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
	         
      }
   } 
   else 
   {
	    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	    MPI_Recv(&bin_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	    MPI_Recv(&min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	    MPI_Recv(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	    MPI_Recv(&randomArray, 100, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
   	 	
   }
 
    

		printf("Rank = %d  Range = %f \n",rank,range );
	
	for ( i = 0; i < n; i++)
	{
		printf("%d \t",&randomArray[i] );
	}


	for ( i = 0; i < n  && i % size==0; i++)
	{
		index=(randomArray[i])/range;
		result[index]= result[index]++;
		max_elem=index<max_elem?max_elem:index;
		
	}
	 
    if(rank=size-1)
    {

		for (int i = 0; i < max_elem; ++i)
		{
			printf("%d ---> %d\n ",i ,result[i]);
		}

    }
    MPI_Bcast(&result, 50, MPI_INT, rank, MPI_COMM_WORLD);
	  MPI_Bcast(&max_elem, 1, MPI_INT, rank, MPI_COMM_WORLD);
    



	MPI_Finalize();

return -1;


}