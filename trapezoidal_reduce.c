#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv) {
    
	double f(double x);
	double Trap (double left_endpt,double right_endpt,int trap_count,double base_len);

    int         rank,size,n,local_n;
    double       a,b,h,local_a,local_b,integral,total;        
   
	
    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
       printf("Enter the value of a :\n");
		scanf("%lf",&a);	
		printf("Enter the value of b :\n");
		scanf("%lf",&b);
		printf("Enter the value of n :\n");
		scanf("%d",&n);
    }
   
 	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	h=(b-a)/n;
	//printf("h --> %f\n", h);
	local_n=n/size;
	//printf("local_n --> %f\n", local_n);
	local_a = a + rank * local_n * h;
	//printf("local_a formulae --> %f\n", a + rank * local_n * h);
	//printf("local_a --> %f\n", local_a);
	local_b = local_a * local_n * h;
	//printf("local_b --> %f\n", local_b);
	integral  +=  Trap(local_a, local_b, local_n, h);
	//printf("integral --> %f\n", integral);
 
    MPI_Reduce(&integral, &total, 1, MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == size-1) {
      printf("with n=%d trapezoida out estimate of the integral from %f to %f = %f\n",n ,a,b,integral);
    }

       MPI_Finalize();
    return 0;
}



double f(double x)
{
	return (x*x);
}

double Trap (double left_endpt,double right_endpt,int trap_count,double base_len)
{
		// printf("%f --> %f  --> %f -->%f \n", left_endpt,right_endpt,trap_count,base_len);
	double estimate, x;
	estimate = (f(left_endpt)+ f(right_endpt))/2.0;
	for (int i = 1; i <= trap_count-1 ; i++)
	{
		x=left_endpt + i * base_len;
		estimate  +=  f(x);
	}
	return estimate *base_len;


}