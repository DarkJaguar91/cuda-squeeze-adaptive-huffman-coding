#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/iterator/constant_iterator.h>
#include <thrust/sort.h>
#include <thrust/inner_product.h>
#include <iostream>
#include <vector>

#include "../Compressor/Compressor.h"

namespace GPULib {
  void compressGPU(frequencyValues & map, float * values, const longValue & numVals); 
}