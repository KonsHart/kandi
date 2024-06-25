#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 512

void fill_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = rand() % 100;
        }
    }
}

void multiply_matrices(int mat1[N][N], int mat2[N][N], int res[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int k = 0; k < N; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main() {
    int mat1[N][N], mat2[N][N], res[N][N];
    fill_matrix(mat1);
    fill_matrix(mat2);

    clock_t start, end;
    double time_taken;

    start = clock();
    multiply_matrices(mat1, mat2, res);
    end = clock();

    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by unoptimized matrix multiplication: %f seconds\n", time_taken);

    return 0;
}
