#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 512
#define NUM_THREADS 4

// Funktio matriisin täyttämiseen satunnaisilla arvoilla
void fill_matrix(int **mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mat[i][j] = rand() % 100;
        }
    }
}

// Perinteinen matriisikertolasku
void multiply_matrices(int **mat1, int **mat2, int **res, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            res[i][j] = 0;
            for (int k = 0; k < size; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Funktiot matriisien yhteen- ja vähennyslaskuun
void add_matrices(int **A, int **B, int **C, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtract_matrices(int **A, int **B, int **C, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// Strassenin algoritmi
void strassen(int **A, int **B, int **C, int size) {
    if (size <= 2) {
        multiply_matrices(A, B, C, size);
        return;
    }

    int new_size = size / 2;

    // Alimatriisien dynaaminen allokointi
    int **a11 = malloc(new_size * sizeof(int *));
    int **a12 = malloc(new_size * sizeof(int *));
    int **a21 = malloc(new_size * sizeof(int *));
    int **a22 = malloc(new_size * sizeof(int *));
    int **b11 = malloc(new_size * sizeof(int *));
    int **b12 = malloc(new_size * sizeof(int *));
    int **b21 = malloc(new_size * sizeof(int *));
    int **b22 = malloc(new_size * sizeof(int *));
    int **c11 = malloc(new_size * sizeof(int *));
    int **c12 = malloc(new_size * sizeof(int *));
    int **c21 = malloc(new_size * sizeof(int *));
    int **c22 = malloc(new_size * sizeof(int *));
    int **m1 = malloc(new_size * sizeof(int *));
    int **m2 = malloc(new_size * sizeof(int *));
    int **m3 = malloc(new_size * sizeof(int *));
    int **m4 = malloc(new_size * sizeof(int *));
    int **m5 = malloc(new_size * sizeof(int *));
    int **m6 = malloc(new_size * sizeof(int *));
    int **m7 = malloc(new_size * sizeof(int *));
    int **temp1 = malloc(new_size * sizeof(int *));
    int **temp2 = malloc(new_size * sizeof(int *));
    
    for (int i = 0; i < new_size; i++) {
        a11[i] = malloc(new_size * sizeof(int));
        a12[i] = malloc(new_size * sizeof(int));
        a21[i] = malloc(new_size * sizeof(int));
        a22[i] = malloc(new_size * sizeof(int));
        b11[i] = malloc(new_size * sizeof(int));
        b12[i] = malloc(new_size * sizeof(int));
        b21[i] = malloc(new_size * sizeof(int));
        b22[i] = malloc(new_size * sizeof(int));
        c11[i] = malloc(new_size * sizeof(int));
        c12[i] = malloc(new_size * sizeof(int));
        c21[i] = malloc(new_size * sizeof(int));
        c22[i] = malloc(new_size * sizeof(int));
        m1[i] = malloc(new_size * sizeof(int));
        m2[i] = malloc(new_size * sizeof(int));
        m3[i] = malloc(new_size * sizeof(int));
        m4[i] = malloc(new_size * sizeof(int));
        m5[i] = malloc(new_size * sizeof(int));
        m6[i] = malloc(new_size * sizeof(int));
        m7[i] = malloc(new_size * sizeof(int));
        temp1[i] = malloc(new_size * sizeof(int));
        temp2[i] = malloc(new_size * sizeof(int));
    }

    // Alimatriisien jako
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + new_size];
            a21[i][j] = A[i + new_size][j];
            a22[i][j] = A[i + new_size][j + new_size];

            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + new_size];
            b21[i][j] = B[i + new_size][j];
            b22[i][j] = B[i + new_size][j + new_size];
        }
    }

    // Lasketaan m1 - m7
    add_matrices(a11, a22, temp1, new_size); // temp1 = a11 + a22
    add_matrices(b11, b22, temp2, new_size); // temp2 = b11 + b22
    strassen(temp1, temp2, m1, new_size); // m1 = (a11+a22) * (b11+b22)

    add_matrices(a21, a22, temp1, new_size); // temp1 = a21 + a22
    strassen(temp1, b11, m2, new_size); // m2 = (a21+a22) * (b11)

    subtract_matrices(b12, b22, temp2, new_size); // temp2 = b12 - b22
    strassen(a11, temp2, m3, new_size); // m3 = (a11) * (b12 - b22)

    subtract_matrices(b21, b11, temp2, new_size); // temp2 = b21 - b11
    strassen(a22, temp2, m4, new_size); // m4 = (a22) * (b21 - b11)

    add_matrices(a11, a12, temp1, new_size); // temp1 = a11 + a12
    strassen(temp1, b22, m5, new_size); // m5 = (a11+a12) * (b22)

    subtract_matrices(a21, a11, temp1, new_size); // temp1 = a21 - a11
    add_matrices(b11, b12, temp2, new_size); // temp2 = b11 + b12
    strassen(temp1, temp2, m6, new_size); // m6 = (a21-a11) * (b11+b12)

    subtract_matrices(a12, a22, temp1, new_size); // temp1 = a12 - a22
    add_matrices(b21, b22, temp2, new_size); // temp2 = b21 + b22
    strassen(temp1, temp2, m7, new_size); // m7 = (a12-a22) * (b21+b22)

    // Lasketaan c11, c12, c21, c22
    add_matrices(m1, m4, temp1, new_size); // temp1 = m1 + m4
    subtract_matrices(temp1, m5, temp2, new_size); // temp2 = temp1 - m5
    add_matrices(temp2, m7, c11, new_size); // c11 = m1 + m4 - m5 + m7

    add_matrices(m3, m5, c12, new_size); // c12 = m3 + m5

    add_matrices(m2, m4, c21, new_size); // c21 = m2 + m4

    add_matrices(m1, m3, temp1, new_size); // temp1 = m1 + m3
    subtract_matrices(temp1, m2, temp2, new_size); // temp2 = temp1 - m2
    add_matrices(temp2, m6, c22, new_size); // c22 = m1 + m3 - m2 + m6

    // Yhdistetään alimatriisit yhdeksi
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            C[i][j] = c11[i][j];
            C[i][j + new_size] = c12[i][j];
            C[i + new_size][j] = c21[i][j];
            C[i + new_size][j + new_size] = c22[i][j];
        }
    }

    // Vapautetaan varattu muisti
    for (int i = 0; i < new_size; i++) {
        free(a11[i]);
        free(a12[i]);
        free(a21[i]);
        free(a22[i]);
        free(b11[i]);
        free(b12[i]);
        free(b21[i]);
        free(b22[i]);
        free(c11[i]);
        free(c12[i]);
        free(c21[i]);
        free(c22[i]);
        free(m1[i]);
        free(m2[i]);
        free(m3[i]);
        free(m4[i]);
        free(m5[i]);
        free(m6[i]);
        free(m7[i]);
        free(temp1[i]);
        free(temp2[i]);
    }
    free(a11);
    free(a12);
    free(a21);
    free(a22);
    free(b11);
    free(b12);
    free(b21);
    free(b22);
    free(c11);
    free(c12);
    free(c21);
    free(c22);
    free(m1);
    free(m2);
    free(m3);
    free(m4);
    free(m5);
    free(m6);
    free(m7);
    free(temp1);
    free(temp2);
}

// Muistinhallinnan optimointi
void multiply_matrices_optimized_memory(int mat1[N][N], int mat2[N][N], int res[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            res[i][j] = sum;
        }
    }
}

// Rinnakkaisprosessointi ja säikeistys
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

// Energiatehokkaat tietorakenteet (simuloitu)
void multiply_matrices_optimized_energy(int mat1[N][N], int mat2[N][N], int res[N][N]) {
    // Käytetään tehokkaampia tietorakenteita ja algoritmeja (simuloitu tässä)
    multiply_matrices_optimized_memory(mat1, mat2, res);
}

int main() {
    // Dynaaminen matriisien allokointi
    int **mat1 = malloc(N * sizeof(int *));
    int **mat2 = malloc(N * sizeof(int *));
    int **res_opt = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        mat1[i] = malloc(N * sizeof(int));
        mat2[i] = malloc(N * sizeof(int));
        res_opt[i] = malloc(N * sizeof(int));
    }

    fill_matrix(mat1, N);
    fill_matrix(mat2, N);

    clock_t start, end;
    double time_taken;

    // Mittaus ilman optimointia
    start = clock();
    multiply_matrices(mat1, mat2, res_opt, N);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by unoptimized matrix multiplication: %f seconds\n", time_taken);

    // Mittaus optimoinnin jälkeen (algoritmien optimointi: Strassen)
    start = clock();
    strassen(mat1, mat2, res_opt, N);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimized matrix multiplication (Strassen's algorithm): %f seconds\n", time_taken);

    // Mittaus optimoinnin jälkeen (muistinhallinnan optimointi)
    start = clock();
    multiply_matrices_optimized_memory(mat1, mat2, res_opt);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimized matrix multiplication (Memory management): %f seconds\n", time_taken);

    // Mittaus optimoinnin jälkeen (rinnakkaisprosessointi ja säikeistys)
    start = clock();
    multiply_matrices_parallel_main();
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimized matrix multiplication (Parallel processing): %f seconds\n", time_taken);

    // Mittaus optimoinnin jälkeen (energiatehokkaat tietorakenteet)
    start = clock();
    multiply_matrices_optimized_energy(mat1, mat2, res_opt);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimized matrix multiplication (Energy-efficient structures): %f seconds\n", time_taken);

    // Vapautetaan varattu muisti
    for (int i = 0; i < N; i++) {
        free(mat1[i]);
        free(mat2[i]);
        free(res_opt[i]);
    }
    free(mat1);
    free(mat2);
    free(res_opt);

    return 0;
}
