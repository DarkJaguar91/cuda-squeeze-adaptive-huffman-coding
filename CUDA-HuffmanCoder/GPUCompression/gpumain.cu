#include "gpumain.h"

double GPUCode::compressGPUlib(const float * floats, frequencyValues & map, const longValue & numVals){
	using namespace std;
	double time = 0;
	thrust::host_vector<float> hostVec(numVals);

	for (longValue i = 0; i < numVals; ++i)
	  hostVec[i] = float(floats[i]);

	Timer::tic();
	thrust::device_vector<float> device_data = hostVec;
	time += Timer::toc();
	cout << "Copying Took: " << Timer::toc() << endl;	

	Timer::tic();
	thrust::sort(device_data.begin(), device_data.end());
	
	int num_bins = thrust::inner_product(device_data.begin(), device_data.end() - 1, device_data.begin() + 1, int(1), thrust::plus<int>(), thrust::not_equal_to<float>());
	
	thrust::device_vector<float> unique_device(num_bins);
	thrust::device_vector<int> counts_device(num_bins);
	
	thrust::reduce_by_key(device_data.begin(), device_data.end(), thrust::constant_iterator<int>(1), unique_device.begin(), counts_device.begin());
	time += Timer::toc();
	cout << "processing Took: " << Timer::toc() << endl;

	Timer::tic();
	thrust::host_vector<float> unique_host = unique_device;
	thrust::host_vector<int> counts_host = counts_device;
	
	for (int i = 0; i < num_bins; ++i)
	  map[unique_host[i]] = new longValue(counts_host[i]);
	time += Timer::toc();
	cout << "mapping Took: " << Timer::toc() << endl;

	return time;
}
