
#include "test_bazel/test_cuda/dim_and_index/print_index.h"


int main(int argc, char **argv) {
  /*
  synchronize是同步的意思，有几种synchronize

  cudaDeviceSynchronize:
  CPU与GPU端完成同步，CPU不执行之后的语句，知道这个语句以前的所有cuda操作结束
  cudaStreamSynchronize:
  跟cudaDeviceSynchronize很像，但是这个是针对某一个stream的。只同步指定的stream中的cpu/gpu操作，其他的不管
  cudaThreadSynchronize: 现在已经不被推荐使用的方法
  __syncthreads:         线程块内同步
  */
  print_one_dim();
  print_two_dim();
  print_cord();
  return 0;
}

