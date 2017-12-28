#include <patternvalidate.h>

// N is the maximum number of structs to insert
#define N 150000
__device__ Match dev_data[N];
__device__ int dev_count = 0;

__device__ PossibleMatches viable_data[N * 10];
__device__ int viable_count = 0;

__device__ int my_push_back(Match & mt) {
	int insert_pt = atomicAdd(&dev_count, 1);
	if (insert_pt < N) {
		dev_data[insert_pt] = mt;
		return insert_pt;
	}
	else return -1;
}

__device__ int insert_viable_pattern(PossibleMatches & mt) {
	int insert_pt = atomicAdd(&viable_count, 1);
	if (insert_pt < N * 10) {
		viable_data[insert_pt] = mt;
		return insert_pt;
	}
	else return -1;
}


__global__ void checkpatternkernal(int img_width, int img_height, unsigned char *img_data, unsigned char *frequency, int max)
{
	int findwdth = blockIdx.y * blockDim.y + threadIdx.y;
	int findhght = blockIdx.x * blockDim.x + threadIdx.x;
	if (findwdth < img_width && findhght < img_height) {
		if (!(findhght > img_height / 4 && findwdth > img_width / 4) && (findhght < img_height && findwdth < img_width) && findhght > img_height / 8 && findwdth > img_width / 8) {
			for (int i = 0; i < img_height; i+=(findhght/2))
			{
				for (int j = 0; j < img_width; j+=(findwdth/2))
				{
					
					int offset_rgb = i*img_width * 3 + j * 3;
					//find position of pattern to be tested
					if (((offset_rgb + findhght*img_width * 3) / (img_width * 3)) == ((offset_rgb + findhght*img_width * 3 + findwdth * 3) / (img_width * 3))
						&& (offset_rgb + findhght*img_width * 3 + findwdth * 3 < img_height*img_width * 3 ) && (i*img_width + j +findhght*img_width+findwdth<img_height*img_width) ) {
						int sidepixel = 100000;
						int similarpixel = 0;
						/*for (int ii = 0; ii < findhght; i++) {
							int offset_gray = i*img_width + j + ii*img_width + 0;
							int offset_gray2 = i*img_width + j + ii*img_width + (findwdth - 1);
							for (int k = 0; k < max; k++) {
								if (img_data[offset_gray] - frequency[k] <= 10 && -10 <= img_data[offset_gray] - frequency[k]) {
									sidepixel++;
									break;
								}
							}
							for (int k = 0; k < max; k++) {
								if (img_data[offset_gray2] - frequency[k] <= 10 && -10 <= img_data[offset_gray2] - frequency[k]) {
									sidepixel++;
									break;
								}
							}
						}
						for (int jj = 0; jj < findwdth; jj++) {
							int offset_gray = i*img_width + j + 0*img_width + jj;
							int offset_gray2 = i*img_width + j + (findhght - 1)*img_width + jj;
							for (int k = 0; k < max; k++) {
								if (img_data[offset_gray] - frequency[k] <= 10 && -10 <= img_data[offset_gray] - frequency[k]) {
									sidepixel++;
									break;
								}
							}
							for (int k = 0; k < max; k++) {
								if (img_data[offset_gray2] - frequency[k] <= 10 && -10 <= img_data[offset_gray2] - frequency[k]) {
									sidepixel++;
									break;
								}
							}
						}*/
						
						if (sidepixel >= (findhght + findwdth)*0.1) {
							for (int ii = 0; ii < findhght; ii++) {
								for (int jj = 0; jj < findwdth; jj++) {
									int offset_gray = i*img_width + j + ii*img_width + jj;
									for (int k = 0; k < max; k++) {
										if (img_data[offset_gray] - frequency[k] <= 6 && -6 <= img_data[offset_gray] - frequency[k]) {
											similarpixel++;
											break;
										}
									}
								}
							}
						}
						if (similarpixel >= findwdth * findhght * 0.35) {
							PossibleMatches temp;
							temp.startpos = offset_rgb;
							temp.findhght = findhght;
							temp.findwdth = findwdth;
							insert_viable_pattern(temp);
						}
					}

				}
			}
		}
	}
}

__global__ void patternkernal(int img_width, int img_height, PossibleMatches *patterns, unsigned char *rgb, int max)
{
	int index = blockIdx.x*blockDim.x + threadIdx.x;
	if (index < max) {
		bool samerow = false;
		double pixeldiffcount = 0;
		int occurences = 0;
		int startpos = patterns[index].startpos;
		int findwdth = patterns[index].findwdth;
		int findhght = patterns[index].findhght;
		//find occurences in image
		for (int ii = 0; ii < img_height; ii += (int)img_height / 33)
		{
			samerow = false;
			for (int jj = 0; jj < img_width; jj += (int)img_width / 33)
			{
				int offset_rgb2 = ii*img_width * 3 + jj * 3;
				pixeldiffcount = 0;
				if (offset_rgb2 + (findhght + 1)*img_width * 3 + (findwdth + 1) * 3 < img_height*img_width * 3 &&
					((offset_rgb2 + findhght*img_width * 3) / (img_width * 3) ==
					(offset_rgb2 + findhght*img_width * 3 + findwdth * 3) / (img_width * 3))) {
					//search within image
					for (int dwn = 0; dwn < findhght; dwn++) {
						for (int rght = 0; rght < findwdth; rght++) {
							unsigned char tr = rgb[offset_rgb2 + dwn*img_width * 3 + rght * 3 + 0];
							unsigned char tg = rgb[offset_rgb2 + dwn*img_width * 3 + rght * 3 + 1];
							unsigned char tb = rgb[offset_rgb2 + dwn*img_width * 3 + rght * 3 + 2];
							unsigned char pr = rgb[startpos + dwn*img_width * 3 + rght * 3 + 0];
							unsigned char pg = rgb[startpos + dwn*img_width * 3 + rght * 3 + 1];
							unsigned char pb = rgb[startpos + dwn*img_width * 3 + rght * 3 + 2];
							pixeldiffcount += (sqrtf(((tr - pr) * (tr - pr) + (tg - pg) * (tg - pg) + (tb - pb) * (tb - pb))));
							if (pixeldiffcount > (findhght*findwdth) * 255 * 0.4f)
								break;
						}
						if (pixeldiffcount > (findhght*findwdth) * 255 * 0.4f)
							break;
						/*if (!sameline)
						break;*/
					}
					//printf("%d\n", samepixel);
					if (pixeldiffcount <= (findhght*findwdth) * 255 * 0.2f) {
						occurences++;
						jj += (findwdth - 1);
						if (!samerow)
							samerow = true;
					}
					else if (pixeldiffcount > findhght*findwdth * 255 * 0.4f && findwdth < img_width / 2)
						jj += (findwdth / 4);
				}
			}
			if (samerow)
				ii += (findhght - 1);
		}
		//printf("2\n");
		//__syncthreads();
		if (occurences >= 4) {
			Match temp;
			temp.startpos = startpos;

			temp.width = findwdth;
			temp.height = findhght;
			temp.times = occurences;
			my_push_back(temp);
		}
	}
}

extern "C" void checkpatterns(
	int img_width,
	int img_height,
	unsigned char *img_data,
	vector<unsigned char>& frequency,
	vector<PossibleMatches>& output)
{
	int size;
	cudaError_t error;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	unsigned char *dimg_data;
	unsigned char *dfrequency;
	error = cudaMalloc((void **)&dimg_data, img_width*img_height * sizeof(unsigned char));
	error = cudaMalloc((void **)&dfrequency, sizeof(PossibleMatches) * frequency.size());
	if (error != cudaSuccess)
		printf("cudaMalloc d_A returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
	error = cudaMemcpy(dimg_data, img_data, img_width*img_height * sizeof(unsigned char), cudaMemcpyHostToDevice);
	error = cudaMemcpy(dfrequency, &*frequency.begin(), sizeof(unsigned char) * frequency.size(), cudaMemcpyHostToDevice);
	int max = frequency.size();
	int block_x_dim = 2000;
	int block_y_dim = 2000;
	dim3 threads_per_block((img_width + 1999) / 2000, (img_height + 1999) / 2000);
	dim3 blocks_per_dimension(block_x_dim, block_y_dim);
	checkpatternkernal << < blocks_per_dimension, threads_per_block >> > (img_width, img_height, dimg_data, dfrequency, max);
	cudaThreadSynchronize();
	cudaDeviceSynchronize();
	error = cudaGetLastError();
	if (error != cudaSuccess)
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	printf("stopped\n");
	cudaMemcpyFromSymbol(&size, viable_count, sizeof(int));
	if (size >= N * 10) { printf("overflow error\n"); size = 150000; }
	vector<PossibleMatches> results(size);
	if (size != 0)
		cudaMemcpyFromSymbol(&(results[0]), viable_data, size * sizeof(PossibleMatches));
	//cudaFree(dev_data);
	printf("first: %d\n", size);
	output = results;
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("first: %.7f\n", milliseconds);
	cudaFree(dimg_data);
	cudaFree(dfrequency);
	cudaDeviceSynchronize();
}

extern "C" void externalfunction(
	int img_width,
	int img_height,
	unsigned char *rgb,
	vector<PossibleMatches>& input,
	vector<Match>& output)
{
	cudaError_t error;
	cudaEvent_t start1, stop1;
	cudaEventCreate(&start1);
	cudaEventCreate(&stop1);
	int size;
	unsigned char *drgb;
	PossibleMatches *dinput;
	error = cudaMalloc((void **)&drgb, img_width*img_height * 3 * sizeof(unsigned char));
	error = cudaMalloc((void **)&dinput, sizeof(PossibleMatches) * input.size());
	if (error != cudaSuccess)
		printf("cudaMalloc d_A returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);

	error = cudaMemcpy(drgb, rgb, img_width*img_height * 3 * sizeof(unsigned char), cudaMemcpyHostToDevice);
	error = cudaMemcpy(dinput, &*input.begin(), sizeof(PossibleMatches) * input.size(), cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
		printf("cudaMalloc d_A returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);

	/*int block_x_dim = 2000;
	int block_y_dim = 2000;
	dim3 threads_per_block((img_width + 1999) / 2000, (img_height + 1999) / 2000);
	dim3 blocks_per_dimension(block_x_dim, block_y_dim);*/
	printf("%d\n", img_width);
	printf("%d\n", img_height);

	int max = input.size();
	cudaEventRecord(start1);
	patternkernal << <(max + 1024 - 1) / 1024, 1024 >> > (img_width, img_height, dinput, drgb, max);
	cudaThreadSynchronize();
	cudaDeviceSynchronize();
	error = cudaGetLastError();
	if (error != cudaSuccess)
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	printf("stopped\n");
	cudaMemcpyFromSymbol(&size, dev_count, sizeof(int));
	if (size >= N) { printf("overflow error\n"); size = 15000; }
	vector<Match> results(size);
	if (size != 0)
		cudaMemcpyFromSymbol(&(results[0]), dev_data, size * sizeof(Match));
	//cudaFree(dev_data);
	printf("%d\n", size);
	output = results;
	cudaEventRecord(stop1);
	cudaEventSynchronize(stop1);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start1, stop1);
	printf("%.7f\n", milliseconds);
	cudaFree(drgb);
	cudaDeviceReset();

}