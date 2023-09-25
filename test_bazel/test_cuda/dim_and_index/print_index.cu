
#include <iostream>

#include <cuda_runtime.h>
#include <stdio.h>

__global__ void print_idx_kernel() {
  printf("block idx: (%3d, %3d, %3d), thread idx: (%3d, %3d, %3d)\n",
         blockIdx.z, blockIdx.y, blockIdx.x, threadIdx.z, threadIdx.y,
         threadIdx.x);
}

__global__ void print_dim_kernel() {
  printf("grid dimension: (%3d, %3d, %3d), block dimension: (%3d, %3d, %3d)\n",
         gridDim.z, gridDim.y, gridDim.x, blockDim.z, blockDim.y, blockDim.x);
}

__global__ void print_thread_idx_per_block_kernel() {
  int index = threadIdx.z * blockDim.x * blockDim.y + threadIdx.y * blockDim.x +
              threadIdx.x;

  printf("block idx: (%3d, %3d, %3d), thread idx: %3d\n", blockIdx.z,
         blockIdx.y, blockIdx.x, index);
}

__global__ void print_thread_idx_per_grid_kernel() {
  int bSize = blockDim.z * blockDim.y * blockDim.x;

  int bIndex =
      blockIdx.z * gridDim.x * gridDim.y + blockIdx.y * gridDim.x + blockIdx.x;

  int tIndex = threadIdx.z * blockDim.x * blockDim.y +
               threadIdx.y * blockDim.x + threadIdx.x;

  int index = bIndex * bSize + tIndex;

  printf("block idx: %3d, thread idx in block: %3d, thread idx: %3d\n", bIndex,
         tIndex, index);
}

__global__ void print_cord_kernel() {
  int index = threadIdx.z * blockDim.x * blockDim.y + threadIdx.y * blockDim.x +
              threadIdx.x;

  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  printf("block idx: (%3d, %3d, %3d), thread idx: %3d, cord: (%3d, %3d)\n",
         blockIdx.z, blockIdx.y, blockIdx.x, index, x, y);
}

void print_one_dim() {
  int inputSize = 8;
  int blockDim = 4;
  int gridDim = inputSize / blockDim;

  dim3 block(blockDim);
  dim3 grid(gridDim);

  /* 这里建议大家吧每一函数都试一遍*/
  // print_idx_kernel<<<grid, block>>>();
  // print_dim_kernel<<<grid, block>>>();
  // print_thread_idx_per_block_kernel<<<grid, block>>>();
  print_thread_idx_per_grid_kernel<<<grid, block>>>();

  cudaDeviceSynchronize();
}

void print_two_dim() {
  int inputWidth = 4;

  int blockDim = 2;
  int gridDim = inputWidth / blockDim;

  dim3 block(blockDim, blockDim);
  dim3 grid(gridDim, gridDim);

  /* 这里建议大家吧每一函数都试一遍*/
  // print_idx_kernel<<<grid, block>>>();
  // print_dim_kernel<<<grid, block>>>();
  // print_thread_idx_per_block_kernel<<<grid, block>>>();
  print_thread_idx_per_grid_kernel<<<grid, block>>>();

  cudaDeviceSynchronize();
}

void print_cord() {
  int inputWidth = 4;

  int blockDim = 2;
  int gridDim = inputWidth / blockDim;

  dim3 block(blockDim, blockDim);
  dim3 grid(gridDim, gridDim);

  print_cord_kernel<<<grid, block>>>();

  cudaDeviceSynchronize();
}
