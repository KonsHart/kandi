#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 512
#define NUM_THREADS 4

int mat1[N][N], mat2[N][N], res[N][N];

void fill_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = rand() % 100;
        }
    }
}

void *multiply_matrices_parallel(void *arg) {
    int thread_id = *(int *)arg;
    int i_start = thread_id * (N / NUM_THREADS);
    int i_end = (thread_id + 1) * (N / NUM_THREADS);
    for (int i = i_start; i < i_end; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            res[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

void multiply_matrices_parallel_main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply_matrices_parallel, (void *)&thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    fill_matrix(mat1);
    fill_matrix(mat2);

    clock_t start, end;
    double time_taken;

    start = clock();
    multiply_matrices_parallel_main();
    end = clock();

    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimized matrix multiplication (Parallel processing): %f seconds\n", time_taken);

    return 0;
}
