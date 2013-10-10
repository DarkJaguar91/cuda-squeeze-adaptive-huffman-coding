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
#include "../HuffmanCoder/Decompressor.h"

namespace GPU {
typedef thrust::system::cuda::experimental::pinned_allocator<float> allocator;
typedef thrust::system::cuda::experimental::pinned_allocator<int> allocatorint;
typedef thrust::host_vector<float, allocator> host_vec;
typedef thrust::host_vector<int, allocatorint> host_vec_int;
typedef thrust::device_vector<float, allocator> dev_vec;
typedef thrust::device_vector<int, allocatorint> dev_vec_int;

void compress(const longValue & numberOfFloats);
void compress(char * inputFileName, char * outputFileName);
void compressData(host_vec & host, frequencyValues & map, std::vector<unsigned char> & charCodes, double & totTime);
}

#endif /* GRAPICSCOMPRESSION_H_ */
