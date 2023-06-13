#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>

#define MATRIX_SIZE 1024

void matmul_naive (double *a, double *b, double *c)
{
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      for (int k = 0; k < MATRIX_SIZE; k++) {
        c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
      }
    }
  }
}


void matmul_optimized_slices (double *a, double *b, double *c)
{
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int num = omp_get_num_threads();
    for (int i = tid; i < MATRIX_SIZE; i+=num) {
      for (int j = 0; j < MATRIX_SIZE; j++) {
        for (int k = 0; k < MATRIX_SIZE; k++) {
          c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }
  }
}

void matmul_optimized_chunks (double *a, double *b, double *c)
{
  #pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    int chunk_size = MATRIX_SIZE / num_threads + 1;
    int tid = omp_get_thread_num();  
    int start = tid * chunk_size;
    int end = (tid + 1) * chunk_size;
    for (int i = start; i < end && i < MATRIX_SIZE; i++) {
      for (int j = 0; j < MATRIX_SIZE; j++) {
        for (int k = 0; k < MATRIX_SIZE; k++) {
          c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }
  }
}

int main()
{
  double *a = (double *) malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *b = (double *) malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *c = (double *) calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));
  double *c2 = (double *) calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));


  if (!a || !b || !c || !c2) {
    printf("Error: could not allocate memory.\n");
    return 1;
  }

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      a[i * MATRIX_SIZE + j] = (double) rand() / RAND_MAX;
      b[i * MATRIX_SIZE + j] = (double) rand() / RAND_MAX;
    }
  }

  double start_time = omp_get_wtime();
  matmul_naive(a, b, c2);
  double end_time = omp_get_wtime();
  printf("Naive: %.5f seconds\n", end_time - start_time);

  start_time = omp_get_wtime();
  matmul_optimized_slices(a, b, c);
  end_time = omp_get_wtime();
  printf("Optimized (slices): %.5f seconds\n", end_time - start_time);

  for (int i = 0; i != MATRIX_SIZE * MATRIX_SIZE; ++i)
  {
    if (fabs(c[i] - c2[i]) > 1e-6)
      {
        printf("slice wrong at %d with %f, %f\n", i, c[i], c2[i]);
        break;
      }
  }

  memset(c, 0, MATRIX_SIZE*MATRIX_SIZE*sizeof(double));

  start_time = omp_get_wtime();
  matmul_optimized_chunks(a, b, c);
  end_time = omp_get_wtime();
  printf("Optimized (chunks): %.5f seconds\n", end_time - start_time);

  for (int i = 0; i != MATRIX_SIZE * MATRIX_SIZE; ++i)
  {
    if (fabs(c[i] - c2[i]) > 1e-6)
      {
        printf("chunk wrong at %d with %f, %f\n", i, c[i], c2[i]);
        break;
      }
  }

  free(a);
  free(b);
  free(c);
  free(c2);

  return 0;
}
