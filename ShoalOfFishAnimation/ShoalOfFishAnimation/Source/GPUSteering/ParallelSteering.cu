#include <iostream>
#include <chrono> 

#include <GL/glew.h>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <string>
#include <sstream>

#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include "ParallelSteering.cuh"
#include "../Constant/Constant.h"

#pragma once

#ifdef NVCC
#define CU_LAUNCH(...) <<<__VA_ARGS__>>>
#else
#define CU_LAUNCH(...) 
#define __launch_bounds__(...)
#define __syncwarp()
#define __syncthreads()
#define __any_sync() (0)
#define __all_sync() (0)
#define __ballot_sync() (0)
#endif

#pragma region Cuda error
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}
#pragma endregion

#pragma region Cuda Vector
__device__ struct CudaVector
{
public:
	float X, Y;
	__device__ CudaVector(float x, float y);
};
__device__ CudaVector::CudaVector(float x, float y)
{
	this->X = x;
	this->Y = y;
}
#pragma endregion

#pragma region Kernel
__global__ void MoveFish(Fish *fish, float mouseX, float mouseY)
{
	int threadIndex = threadIdx.x;
	int blockIndex = blockIdx.x;
	threadIndex = (blockIndex * 1024) + threadIndex;
	if (threadIndex < FISH_COUNT)
	{
		float fishPositionX = fish[threadIndex].Position.X;
		float fishPositionY = fish[threadIndex].Position.Y;
		float fishSize = fish[threadIndex].Settings.size;

		CudaVector averageDirection = CudaVector(fish[threadIndex].Direction.X, fish[threadIndex].Direction.Y);
		CudaVector groupingDirection = CudaVector(0, 0);
		CudaVector antyCrowdingVector = CudaVector(0, 0);
		#pragma region Steering
		{
			CudaVector groupingPoint = CudaVector(fishPositionX, fishPositionY);
			int n = 1;
			for (int i = 0; i < FISH_COUNT; i++)
			{
				if (i == threadIndex) continue;
				float x = fish[i].Position.X;
				float y = fish[i].Position.Y;
				float distance_X = (fishPositionX - x);
				float distance_Y = (fishPositionY - y);
				float distance = sqrt(distance_X*distance_X+distance_Y*distance_Y);
				if ( distance < FISH_VIEW_RANGE)
				{
					#pragma region SteerToTheAverageHeadingOfLocalFlockmates
					if (!fish[threadIndex].Settings.independence)
					{
						averageDirection.X += fish[i].Direction.X;
						averageDirection.Y += fish[i].Direction.Y;
					}
					#pragma endregion

					#pragma region SteerToTheAveragePositionOfLocalFlockmates
					if (fish[threadIndex].Settings.grouping)
					{
						groupingPoint.X += x;
						groupingPoint.Y += y;
						n++;
					}
					#pragma endregion
				}
				#pragma region  SteerToAvoidCrowdingLocalFlockmates
				if (fish[threadIndex].Settings.grouping)
				{
					float colisionRange = (FISH_COLISION_RANGE * fish[i].Settings.size) + (FISH_COLISION_RANGE * fishSize);
					if (distance < colisionRange)
					{
						antyCrowdingVector.X += (((colisionRange*distance_X) / distance) - distance_X);
						antyCrowdingVector.Y += (((colisionRange*distance_Y) / distance) - distance_Y);
					}
				}
				#pragma endregion
			}
			#pragma region SteerToTheAverageHeadingOfLocalFlockmates
			float length = sqrt(averageDirection.X*averageDirection.X+averageDirection.Y*averageDirection.Y);
			if (!fish[threadIndex].Settings.independence)
			{
				if (length > 0.0001)
				{
					averageDirection.X /= length;
					averageDirection.Y /= length;
				}
				else
				{
					averageDirection.X = fish[threadIndex].Direction.X;
					averageDirection.Y = fish[threadIndex].Direction.Y;
				}
			}
			#pragma endregion

			#pragma region SteerToTheAveragePositionOfLocalFlockmates
			if (fish[threadIndex].Settings.grouping)
			{
				groupingPoint.X /= n;
				groupingPoint.Y /= n;
				groupingDirection.X = (groupingPoint.X - fishPositionX);
				groupingDirection.Y = (groupingPoint.Y - fishPositionY);
				length = sqrt(groupingDirection.X*groupingDirection.X + groupingDirection.Y*groupingDirection.Y);
				if (length > 0.001)
				{
					groupingDirection.X /= length;
					groupingDirection.Y /= length;
				}
			}
			#pragma endregion
		}
		#pragma endregion
		CudaVector avoidMouse = CudaVector(0, 0);
		#pragma region AvoidingMouse
		float dist = sqrt((mouseX - fishPositionX)*(mouseX - fishPositionX) + (mouseY - fishPositionY)*(mouseY - fishPositionY));
		if (dist <= MOUSE_FEAR_DISTANCE)
		{
			avoidMouse.X = ((fishPositionX - mouseX) / dist) * (MOUSE_FEAR_DISTANCE - dist);
			avoidMouse.Y = ((fishPositionY - mouseY) / dist) * (MOUSE_FEAR_DISTANCE - dist);
		}
		#pragma endregion



		__syncthreads();
		float x_move = groupingDirection.X + antyCrowdingVector.X * 0.3 + averageDirection.X * fish[threadIndex].Settings.speed + avoidMouse.X;
		float y_move = groupingDirection.Y + antyCrowdingVector.Y * 0.3 + averageDirection.Y * fish[threadIndex].Settings.speed + avoidMouse.Y;
		fish[threadIndex].Position.X += x_move;
		fish[threadIndex].Position.Y += y_move;

#pragma region Window frame
		if (fish[threadIndex].Position.X > MATRIX_HALF_WIDTH)
		{
			fish[threadIndex].Position.X = -MATRIX_HALF_WIDTH;
			fish[threadIndex].Position.Y *= -1;
		}
		if (fish[threadIndex].Position.X < -MATRIX_HALF_WIDTH)
		{
			fish[threadIndex].Position.X = MATRIX_HALF_WIDTH;
			fish[threadIndex].Position.Y *= -1;
		}
		if (fish[threadIndex].Position.Y > MATRIX_HALF_HEIGHT)
		{
			fish[threadIndex].Position.X *= -1;
			fish[threadIndex].Position.Y = -MATRIX_HALF_HEIGHT;
		}
		if (fish[threadIndex].Position.Y < -MATRIX_HALF_HEIGHT)
		{
			fish[threadIndex].Position.X *= -1;
			fish[threadIndex].Position.Y = MATRIX_HALF_HEIGHT;
		}
#pragma endregion

		float directionLength = sqrt((averageDirection.X + avoidMouse.X) * (averageDirection.X + avoidMouse.X) + (averageDirection.Y + avoidMouse.Y)*(averageDirection.Y + avoidMouse.Y));
		if (directionLength > 0.0001)
		{
			fish[threadIndex].Direction.X = (averageDirection.X + avoidMouse.X) / directionLength;
			fish[threadIndex].Direction.Y = (averageDirection.Y + avoidMouse.Y) / directionLength;
		}
		else
		{
			fish[threadIndex].Direction.X = averageDirection.X;
			fish[threadIndex].Direction.Y = averageDirection.Y;
		}
	}

}
#pragma endregion

extern "C"  std::string ParallelSteering(Fish* h_fish, float MouseX, float MouseY) 
{
	Fish* d_fish;
	double mallocTime = 0;
	double calcualtionTime = 0;

	auto start = std::chrono::high_resolution_clock::now();
	#pragma region HostToDevice
		cudaMalloc((void**)&d_fish, FISH_COUNT * sizeof(Fish));
		cudaMemcpy(d_fish, h_fish, FISH_COUNT * sizeof(Fish), cudaMemcpyHostToDevice);
	#pragma endregion
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedMem = (finish - start);

	start = std::chrono::high_resolution_clock::now();
	#pragma region Calcualtion
		MoveFish << <1 + (FISH_COUNT / 1024), 1024 >> > (d_fish, MouseX, MouseY);
		gpuErrchk(cudaPeekAtLastError());
		cudaThreadSynchronize();
	#pragma endregion
	finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedCal = (finish - start);

	start = std::chrono::high_resolution_clock::now();
	#pragma region DeviceToHost
		cudaMemcpy(h_fish, d_fish, FISH_COUNT * sizeof(Fish), cudaMemcpyDeviceToHost);
		cudaFree(d_fish);
	#pragma endregion	
	finish = std::chrono::high_resolution_clock::now();
	elapsedMem += (finish - start);

	std::stringstream streams;
	streams << std::fixed << "| Calcualtion time : " << elapsedCal.count() << "s | Data copying time: " << elapsedMem.count() << "s ";

	return streams.str();
}

