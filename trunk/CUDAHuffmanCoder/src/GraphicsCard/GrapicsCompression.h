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

#include "../Timer/Timer.h"
#include "../HuffmanCoder/Compressor.h"

namespace GPU {
	void compress(const longValue & numberOfFloats);
	void binVector(thrust::device_vector<float> & dev_vec, frequencyValues & map,
			double & time);
}

#endif /* GRAPICSCOMPRESSION_H_ */
