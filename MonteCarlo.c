#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
 
int main(int argc, char* argv[])
{
    int n = 100000,rank,size,i,ct=0,insideCount,totalN;                      
    double x,y,point,pi;                    
    MPI_Init(&argc, &argv);                
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   
    MPI_Comm_size(MPI_COMM_WORLD,&size);
        
 
    if(rank != 0)
    {
        for (i=0; i<n; ++i)                
        {
            x = (double)random()/RAND_MAX;     
            y = (double)random()/RAND_MAX;     
            point = sqrt((x*x)+(y*y));             
            if (point<=1)
                ++ct;                
            
        }
    }
 
    MPI_Reduce(&ct, &insideCount, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
    MPI_Reduce(&n, &totalN,  1,  MPI_INT, MPI_SUM,  0, MPI_COMM_WORLD);
    totalN -= n;                
 
    if (rank == 0)                     
    {
        pi = ((double)insideCount/(double)totalN)*4.0;               
        printf("Pi: %f\nNumber of Points inside circle %i\nNumber of iteration  %d\n", pi, insideCount, totalN);
     
 
    }
 
    MPI_Finalize();                    
    return 0;
}