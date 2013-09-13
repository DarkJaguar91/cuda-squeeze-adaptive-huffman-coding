#include "GPUmain.h"

void GPUCode::compressGPU() {
  thrust::host_vector<float> h(1000000);
  thrust::device_vector<float> d;
  
  thrust::generate(h.begin(), h.end(), rand);
  
  for (int i = 0; i < 1000000; ++i)
    std::cout << h[i] << " ";
  std::cout << std::endl;
  
  d = h;
  
  std::cout << "Done" << std::endl;
} 