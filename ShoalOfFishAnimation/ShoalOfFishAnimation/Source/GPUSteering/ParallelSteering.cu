#include <iostream>

#include <GL/glew.h>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include "ParallelSteering.cuh"

__device__ struct CudaVector
{
	float x, y;
	CudaVector(float x = 0, float y = 0);
	
	
};
__device__ CudaVector::CudaVector(float x, float y)
{
	this->x = x;
	this->y = y;
}

__global__ void MoveFish(Fish *fish, int count)
{
	int threadIndex = threadIdx.x;
	if (threadIndex < count)
	{
		/*Vector v = fish[threadIndex].VectorToTheAverageHeadingOfLocalFlockmates(fish, count);
		fish[threadIndex].MoveBy(v);*/
		//Takieo takie = Takieo(0, 2);
		fish[threadIndex].Position.X += fish[threadIndex].Direction.X;
		fish[threadIndex].Position.Y += fish[threadIndex].Direction.Y;
	}

}

extern "C" void ParallelSteering(Fish* h_fish, int count)
{
	Fish* d_fish;
	cudaMalloc((void**)&d_fish, count * sizeof(Fish));
	cudaMemcpy(d_fish, h_fish, count * sizeof(Fish), cudaMemcpyHostToDevice);
	MoveFish << <1, count >> > (d_fish, count);
	cudaThreadSynchronize();
	cudaMemcpy(h_fish, d_fish, count * sizeof(Fish), cudaMemcpyDeviceToHost);
	cudaFree(d_fish);
}

