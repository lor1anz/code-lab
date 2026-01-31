#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>

#define N 256
#define M 512
#define K 256
#define BLOCK_SIZE 32

// naive realization (no loops unrolling, no blocks, no parallel)
void matmul_cpu(float *A, float *B, float *C, int n, int m, int k) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            float sum = 0.0f;
            for (int l = 0; l < m; ++l) {
                sum += A[i * m + l] * B[l * k + j];
            }
            C[i * k + j] = sum;
        }
    }
}

__global__ void matmul_gpu(float *A, float *B, float *C, int n, int m, int k) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < k) {
        float sum = 0.0f;
        for (int l = 0; l < m; ++l) {
            sum += A[row * m + l] * B[l * k + col];
        }
        C[row * k + col] = sum;
    }
}

void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    float *h_A, *h_B, *h_C_cpu, *h_C_gpu;
    float *d_A, *d_B, *d_C;
    int size_A = N * M * sizeof(float);
    int size_B = M * K * sizeof(float);
    int size_C = N * K * sizeof(float);

    h_A = (float*)malloc(size_A);
    h_B = (float*)malloc(size_B);
    h_C_cpu = (float*)malloc(size_C);
    h_C_gpu = (float*)malloc(size_C);

    srand(time(NULL));
    init_matrix(h_A, N, M);
    init_matrix(h_B, M, K);

    cudaMalloc(&d_A, size_A);
    cudaMalloc(&d_B, size_B);
    cudaMalloc(&d_C, size_C);

    cudaMemcpy(d_A, h_A, size_A, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size_B, cudaMemcpyHostToDevice);

    dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
    dim3 gridDim((K + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);

    printf("Performing warm-up runs...\n");
    for (int i = 0; i < 3; ++i) {
        matmul_cpu(h_A, h_B, h_C_cpu, M, K, N);
        matmul_gpu<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, K, N);
        cudaDeviceSynchronize();
    }

    printf("Benchmarking CPU implementation...\n");
    double cpu_total_time = 0.0;
    for (int i = 0; i < 20; ++i) {
        double start_time = get_time();
        matmul_cpu(h_A, h_B, h_C_cpu, N, M, K);
        double end_time = get_time();
        cpu_total_time += end_time - start_time;
    }
    double cpu_avg_time = cpu_total_time / 20.0;

    printf("Benchmarking GPU implementation...\n");
    double gpu_total_time = 0.0;
    for (int i = 0; i < 20; ++i) {
        double start_time = get_time();
        matmul_gpu<<<gridDim, blockDim>>>(d_A, d_B, d_C, N, M, K);
        cudaDeviceSynchronize();
        double end_time = get_time();
        gpu_total_time += end_time - start_time;
    }
    double gpu_avg_time = gpu_total_time / 20.0;

    printf("CPU average time: %f microseconds\n", (cpu_avg_time * 1e6f));
    printf("GPU average time: %f microseconds\n", (gpu_avg_time * 1e6f));
    printf("Speedup: %fx\n", cpu_avg_time / gpu_avg_time);

    cudaMemcpy(h_C_gpu, d_C, size_C, cudaMemcpyDeviceToHost);
    bool is_correct = true;
    for (int i = 0; i < N * K; ++i) {
        if (fabs(h_C_cpu[i] - h_C_gpu[i]) > 1e-5) {
            is_correct = false;
            break;
        }
    }
    printf("Results are %s\n", is_correct ? "correct" : "incorrect");

    free(h_A);
    free(h_B);
    free(h_C_cpu);
    free(h_C_gpu);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return 0;
}
