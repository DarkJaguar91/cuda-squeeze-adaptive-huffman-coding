#include <vector>
#include <algorithm>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/sort.h>
#include <thrust/inner_product.h>
#include <thrust/iterator/constant_iterator.h>

#include "../HuffmanCoder/Compressor.h"
#include "../Timer/Timer.h"

namespace GPUCode {
  void compressGPU(const float * floats, frequencyValues & map, const longValue & numVal);
}