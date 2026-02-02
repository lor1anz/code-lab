#include <cuda_runtime.h>
#include <nvtx3/nvToolsExt.h>
#include <iostream>

#define N 1024
#define BLOCK_SIZE 16

__global__ void matrix_mult_kernel(float *A, float *B, float *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    float sum = 0.0f;

    if (row < n && col < n) {
        for (int i = 0; i < n; ++i) {
            sum += A[row * n + i] * B[i * n + col];
        }
        C[row * n + col] = sum;
    }
}

void matrix_mult(float *A, float *B, float *C, int n) {
    nvtxRangePush("Matrix Multiplication");

    float *d_A, *d_B, *d_C;
    int size = n * n * sizeof(float);

    nvtxRangePush("Memory Allocation");
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);
    nvtxRangePop();

    nvtxRangePush("Memory Copy H2D");
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
    nvtxRangePop();

    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 numBlocks((n + BLOCK_SIZE - 1) / BLOCK_SIZE, (n + BLOCK_SIZE - 1) / BLOCK_SIZE);

    nvtxRangePush("Kernel Execution");
    matrix_mult_kernel<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, n);
    cudaDeviceSynchronize();
    nvtxRangePop();

    nvtxRangePush("Memory Copy D2H");
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    nvtxRangePop();

    nvtxRangePush("Memory Deallocation");
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    nvtxRangePop();

    nvtxRangePop();
}

int main() {
    const int n = N;
    float *A = new float[n * n];
    float *B = new float[n * n];
    float *C = new float[n * n];

    matrix_mult(A, B, C, n);

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}