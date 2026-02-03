#include <stdio.h>
#include <stdlib.h>
#include <nvtx3/nvToolsExt.h>
#include <cuda_runtime.h>

#define N 1000000
#define BLOCK_SIZE 256

__global__ void vector_add_gpu(int *a, int *b, int *c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

void vector_add_cpu(int *a, int *b, int *c, int n)
{
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    nvtxRangePush("Vector Add Events");

    int *h_a, *h_b, *h_c_cpu, *h_c_gpu;
    int *d_a, *d_b, *d_c;
    size_t size = N * sizeof(float);
    cudaStream_t s1, s2;
    cudaEvent_t event;

    h_a = (int*)malloc(size);
    h_b = (int*)malloc(size);
    h_c_cpu = (int*)malloc(size);
    h_c_gpu = (int*)malloc(size);

    memset(h_c_cpu, 0, size);
    memset(h_c_gpu, 0, size);
    for (int i = 0; i < N; ++i) {
        h_a[i] = h_b[i] = i;
    }

    int low, high;
    cudaDeviceGetStreamPriorityRange(&low, &high);

    cudaStreamCreateWithPriority(&s1, cudaStreamNonBlocking, high);
    cudaStreamCreateWithPriority(&s2, cudaStreamNonBlocking, low);

    cudaEventCreate(&event);

    cudaMallocAsync(&d_a, size, s1);
    cudaMallocAsync(&d_b, size, s1);
    cudaMallocAsync(&d_c, size, s1);

    cudaEventRecord(event, s1);
    cudaStreamWaitEvent(s2, event);

    cudaMemcpyAsync(d_a, h_a, size, cudaMemcpyHostToDevice, s2);
    cudaMemcpyAsync(d_b, h_b, size, cudaMemcpyHostToDevice, s2);

    int num_blocks = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;

    cudaEventRecord(event, s2);
    cudaStreamWaitEvent(s1, event);

    cudaStreamSynchronize(s1);
    cudaStreamSynchronize(s1);

    cudaEventRecord(event, s1);
    cudaStreamWaitEvent(s2, event);

    vector_add_gpu<<<num_blocks, BLOCK_SIZE, 0, s2>>>(d_a, d_b, d_c, N);

    cudaEventRecord(event, s2);
    cudaStreamWaitEvent(s1, event);

    cudaMemcpyAsync(h_c_gpu, d_c, size, cudaMemcpyDeviceToHost, s1);

    vector_add_cpu(h_a, h_b, h_c_cpu, N);

    bool is_correct = true;
    for (int i = 0; i < N; ++i) {
        if (h_c_cpu[i] != h_c_gpu[i]) {
            is_correct = false;
            break;
        }
    }
    printf("Results are %s\n", is_correct ? "correct" : "incorrect");

    free(h_a);
    free(h_b);
    free(h_c_cpu);
    free(h_c_gpu);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    nvtxRangePop();
    return 0;
}
