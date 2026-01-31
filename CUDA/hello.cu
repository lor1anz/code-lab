#include <cstdio>

__global__ void hello_kernel()
{ 
   printf("hello from kernel\n"); 
}

int main() 
{ 
    hello_kernel<<<1,1>>>(); 
    cudaDeviceSynchronize(); 
    return 0; 
}
