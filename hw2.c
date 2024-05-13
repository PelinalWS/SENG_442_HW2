#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define FUNCTION(x) (x*x*x*x + x*x + 4)/(2*x*x*x + x) //can define any function

double integral(double a, double b, int n);

int main(int argc, char** argv){
    float lower = atof(argv[1]);        //takes the left border as the first argument
    float upper = atof(argv[2]);        //takes the right border as the second argument
    int step = atoi(argv[3]);           //takes the step amount as the third argument
    int thread_count = atoi(argv[4]);   //takes the thread count for the last argument
    double result;                      //will return the function call to this
    omp_set_num_threads(thread_count);  //set the thread amount omp will use
    printf("%d threads\n", omp_get_max_threads());          //it needs to be "max" to get the count outside of parallel execution
    result = integral(lower, upper, step);                  //call the function
    printf("The result of the integral: %lf\n", result);    //print result
}

double integral(double a, double b, int n){     //  a -> left bound  |  b -> right bound  | n -> 
    double h = (b - a) / n;                     //calculate the length of step
    double sum = FUNCTION(a) + FUNCTION(b);     //trapezoid calculation
    #pragma omp parallel for reduction(+:sum)   //CS is sum
    for(int i = 1; i < n; i++){         //for each step do the below calculation
        double x = a + i * h;           //x the next x is calculated
        if(i % 2 == 0){                 //the even and odd steps are weighted
            sum += 2.0 * FUNCTION(x);
        } else {
            sum += 4.0 * FUNCTION(x);
        }
    }
    return sum * h / 3.0;               //takes the average and is the namesake of 1/3 rule
}
//gcc -fopenmp -O3 hw2.c -o hw2.exe
//.\hw2.exe -10 67 10000000000 2