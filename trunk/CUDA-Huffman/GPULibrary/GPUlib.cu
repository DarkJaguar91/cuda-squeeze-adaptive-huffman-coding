#include "GPUlib.h"

void GPULib::compressGPU(frequencyValues & map, float * values, const longValue & numVals) {
  // copy data to device
  thrust::host_vector<float> host_data(numVals);
  
  for (longValue i = 0; i < numVals; ++i)
    host_data[i] = values[i];
  
  std::cout << "Loaded host vector" << std::endl;
  
  thrust::device_vector<float> device_data = host_data;  
  
  std::cout << "Copied to device" << std::endl;
  
  // sort vector
  thrust::sort(device_data.begin(), device_data.end());
  
  // count the number of bins (unique values)
  int num_bins = thrust::inner_product(device_data.begin(), device_data.end() - 1, device_data.begin() + 1, int(1), thrust::plus<int>(), thrust::not_equal_to<float>());
  
  // generate new lists
  thrust::device_vector<float> unique_device(num_bins);
  thrust::device_vector<int> counts_device(num_bins);
  
  // remove duplicates and count keys
  thrust::reduce_by_key(device_data.begin(), device_data.end(), thrust::constant_iterator<int>(1), unique_device.begin(), counts_device.begin());
  
  // copy back to CPU
  thrust::host_vector<float> unique_host = unique_device;
  thrust::host_vector<int> counts_host = counts_device;
  
  for (int i = 0; i < num_bins; ++i)
    map[unique_host[i]] = new longValue(counts_host[i]);
} 