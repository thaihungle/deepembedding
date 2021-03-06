#include "utils.h"

/*
 * given log(a) and log(b), return log(a + b)
 *
 */

double log_sum(double log_a, double log_b)
{
  double v;

  if (log_a < log_b)
  {
      v = log_b+log(1 + exp(log_a-log_b));
  }
  else
  {
      v = log_a+log(1 + exp(log_b-log_a));
  }
  return(v);
}

 /**
   * Proc to calculate the value of the trigamma, the second
   * derivative of the loggamma function. Accepts positive matrices.
   * From Abromowitz and Stegun.  Uses formulas 6.4.11 and 6.4.12 with
   * recurrence formula 6.4.6.  Each requires workspace at least 5
   * times the size of X.
   *
   **/

double trigamma(double x)
{
    double p;
    int i;

    x=x+6;
    p=1/(x*x);
    p=(((((0.075757575757576*p-0.033333333333333)*p+0.0238095238095238)
         *p-0.033333333333333)*p+0.166666666666667)*p+1)/x+0.5*p;
    for (i=0; i<6 ;i++)
    {
        x=x-1;
        p=1/(x*x)+p;
    }
    return(p);
}


/*
 * taylor approximation of first derivative of the log gamma function
 *
 */

double digamma(double x)
{
    double p;
    x=x+6;
    p=1/(x*x);
    p=(((0.004166666666667*p-0.003968253986254)*p+
	0.008333333333333)*p-0.083333333333333)*p;
    p=p+log(x)-0.5/x-1/(x-1)-1/(x-2)-1/(x-3)-1/(x-4)-1/(x-5)-1/(x-6);
    return p;
}


double log_gamma(double x)
{
     double z=1/(x*x);

    x=x+6;
    z=(((-0.000595238095238*z+0.000793650793651)
	*z-0.002777777777778)*z+0.083333333333333)/x;
    z=(x-0.5)*log(x)-x+0.918938533204673+z-log(x-1)-
	log(x-2)-log(x-3)-log(x-4)-log(x-5)-log(x-6);
    return z;
}



/*
 * make directory
 *
 */

void make_directory(char* name)
{
    mkdir(name, S_IRUSR|S_IWUSR|S_IXUSR);
}


/*
 * argmax
 *
 */

int argmax(double* x, int n)
{
    int i;
    double max = x[0];
    int argmax = 0;
    for (i = 1; i < n; i++)
    {
        if (x[i] > max)
        {
            max = x[i];
            argmax = i;
        }
    }
    return(argmax);
}

void L1_normalize(double *vec, int dim)
{
	int i;	double sum=0;
	for (i=0; i< dim; i++)	sum += vec[i];
	for (i=0; i< dim; i++)	vec[i] /= sum;
}

void sparsify_vector(double *vec, int dim, int nnz)
{	//sparsify the vector to keep only nnz non-zero elements
    int i, j, ind;		double max;
	char *index;

	index = malloc(sizeof(char)*dim);
	for (i = 0; i < dim; i++) index[i] =0;

	for (i = 0; i < nnz; i++)
	{
		max = 0;
		for (j = 0; j < dim; j++) 
			if (index[j] <1 && vec[j] > max) {max = vec[j]; ind = j; }
		index[ind] =1;
	}
	for (i = 0; i < dim; i++) 
		if (index[i] ==0) vec[i] = 0;
    free(index);
}

void correct_zero_terms(double **bb, int num_terms, int num_topics)
{//if there is a term that does not contribute any to all topics, 
	//we will correct it to contribute a very small amount to all topics
    int i, j, *temp;
    temp = malloc(sizeof(int)*num_terms);
	for (j = 0; j < num_terms; j++) temp[j] = 0;

	for (j = 0; j < num_terms; j++)
		for (i = 0; i < num_topics; i++)
			if (bb[i][j] > 0) { temp[j] = 1; break; }
	for (j = 0; j < num_terms; j++) 
		if (temp[j] == 0) 
			for (i = 0; i < num_topics; i++) bb[i][j] = 1e-10;
	free(temp);
}

void matrix_initialize_sparse_L1(double **bb, int rows, int columns, float t_sparse)
{   int i, j, k, nnz;
	
	nnz = (int)round(t_sparse * columns);
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++) bb[i][j] = 0;
		for (j = 0; j < nnz; j++) { k = rand()% columns; bb[i][k] = rand(); }
		L1_normalize(bb[i], columns);
	}
}

void matrix_random_initialize_L1(double **bb, int rows, int columns)
{   int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++) bb[i][j] = rand();
		L1_normalize(bb[i], columns);
	}
}

double** matrix_initialize(int rows, int columns)
{   int d;	double **bb;
	bb = malloc(sizeof(double*)*rows);
	if (bb == 0)	
	{
		printf("\n matrix_initialize: failed to allocate memory! \n"); 
		printf("rows = %d; columns = %d \n", rows, columns);
		exit(0); 
	}
    for (d = 0; d < rows; d++)	{
		bb[d]		= malloc(sizeof(double) * columns);
		if (bb[d] == 0)	{printf("\n matrix_initialize %d: failed to allocate memory!", d); exit(0); }
	}
return (bb);
}

void clear_vector(double *vec, int dim)
{	//clear all elements to be zero
	int i;
	for (i = 0; i < dim; i++) vec[i] =0;
}

