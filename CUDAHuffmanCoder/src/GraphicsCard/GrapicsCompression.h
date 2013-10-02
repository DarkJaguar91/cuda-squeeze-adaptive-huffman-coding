/*
 * GrapicsCompression.h
 *
 *  Created on: Sep 24, 2013
 *      Author: bjtal
 */

#ifndef GRAPICSCOMPRESSION_H_
#define GRAPICSCOMPRESSION_H_

#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/inner_product.h>
#include <thrust/iterator/constant_iterator.h>
#include <omp.h>
#include <stdio.h>
#include <thrust/system/cuda/experimental/pinned_allocator.h>


#include "../Timer/Timer.h"
#include "../HuffmanCoder/Compressor.h"

namespace GPU {
typedef thrust::system::cuda::experimental::pinned_allocator<float> allocator;
typedef thrust::system::cuda::experimental::pinned_allocator<int> allocatorint;
typedef thrust::host_vector<float> host_vec;
typedef thrust::host_vector<int> host_vec_int;
typedef thrust::device_vector<float> dev_vec;
typedef thrust::device_vector<int> dev_vec_int;

void compress(const longValue & numberOfFloats);
}

#endif /* GRAPICSCOMPRESSION_H_ */
