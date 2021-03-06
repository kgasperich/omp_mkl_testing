#include <iostream>
#include "mkl.h"
#include "mkl_omp_offload.h"

#define SIZE 1024


bool almost_equal(float x, float gold, float tol) {
  return gold * (1-tol) <= x && x <= gold * (1 + tol);
}


int main( int argc, char* argv[] )
{

  float *x = (float *)malloc( sizeof(float)*SIZE);

  float *y = (float *)malloc( sizeof(float)*SIZE);




  float result_cpu = 0;
  float result_gpu = 0;


  const int incx = 1;
  const int incy = 1;

  for(int i=0;i<SIZE;i++)
    {

      x[i] = 1;

      y[i] = 1;


    }


   
   result_cpu   = cblas_sdot(SIZE, x, incx, y, incx);
   
   result_cpu   = cblas_sdot(SIZE, x, incx, y, incx);
   


#pragma omp target data map(to:x[0:SIZE],y[0:SIZE])
    {
      #pragma omp target variant dispatch use_device_ptr(x, y) 

   result_gpu = cblas_sdot(SIZE, x, incx, y, incx);

    }


  if (!almost_equal(result_gpu, result_cpu, 0.1)) {
    std::cerr << "Expected: " << result_cpu << " Got: " << result_gpu << std::endl;
    std::exit(112);
  }


  return 0;
}