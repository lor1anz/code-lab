#include <stdio.h>
#include <cuda_runtime.h>

#define THREADS 1024
#define BLOCKS 1024

__global__ void increment_counter_non_atomic(int *counter, int value)
{
    // no lock
    int old_value = *counter;
    int new_value = old_value + value;
    //no unlock
    *counter = new_value;
}

__global__ void increment_counter_atomic(int *counter, int value)
{
    atomicAdd(counter, value);
}


int main()
{
    int h_counter_non_atomic = 0;
    int h_counter_atomic = 0;
    int *d_counter_non_atomic;
    int *d_counter_atomic;
    int value = 1;

    cudaMalloc(&d_counter_non_atomic, sizeof(int));
    cudaMalloc(&d_counter_atomic, sizeof(int));

    cudaMemcpy(d_counter_non_atomic, &h_counter_non_atomic, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_counter_atomic, &h_counter_atomic, sizeof(int), cudaMemcpyHostToDevice);

    increment_counter_non_atomic<<<BLOCKS, THREADS>>>(d_counter_non_atomic, value);
    increment_counter_atomic<<<BLOCKS, THREADS>>>(d_counter_atomic, value);

    cudaMemcpy(&h_counter_non_atomic, d_counter_non_atomic, sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(&h_counter_atomic, d_counter_atomic, sizeof(int), cudaMemcpyDeviceToHost);

    printf("non_atomic: %d\n", h_counter_non_atomic);
    printf("atomic: %d\n", h_counter_atomic);

    printf("\ncorrect: %d\n", 1024 * 1024);

    cudaFree(d_counter_non_atomic);
    cudaFree(d_counter_atomic);
    return 0;
}
