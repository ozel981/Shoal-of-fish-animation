#include <iostream>
#include <chrono> 

#include <GL/glew.h>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include "ParallelSteering.cuh"
#include "../Constant/Costant.h"

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

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

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

__global__ void MoveFish(Fish *fish, float mouseX, float mouseY)
{
	int threadIndex = threadIdx.x;
	int blockIndex = blockIdx.x;
	threadIndex = (blockIndex * 1024) + threadIndex;
	if (threadIndex < FISH_COUNT)
	{
		float fishPositionX = fish[threadIndex].Position.X;
		float fishPositionY = fish[threadIndex].Position.Y;

		CudaVector averageDirection = CudaVector(fish[threadIndex].Direction.X, fish[threadIndex].Direction.Y);
		#pragma region SteerToTheAverageHeadingOfLocalFlockmates
		if(!fish[threadIndex].Settings.independence)
		{
			for (int i = 0; i < FISH_COUNT; i++)
			{
				if (i == threadIndex) continue;
				float x = fish[i].Position.X;
				float y = fish[i].Position.Y;
				if ((fishPositionX - x)*(fishPositionX - x) + (fishPositionY - y)*(fishPositionY - y) < FISH_VIEW_RANGE*FISH_VIEW_RANGE)
				{
					//avarageDirection += fishes[i].Direction;
					averageDirection.X += fish[i].Direction.X;
					averageDirection.Y += fish[i].Direction.Y;
				}
			}
			float length = sqrt(averageDirection.X*averageDirection.X + averageDirection.Y * averageDirection.Y);
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
		CudaVector groupingDirection = CudaVector(0, 0);
		#pragma region SteerToTheAveragePositionOfLocalFlockmates
		if(fish[threadIndex].Settings.grouping)
		{
			CudaVector groupingPoint = CudaVector(fishPositionX, fishPositionY);
			int n = 1;
			for (int i = 0; i < FISH_COUNT; i++)
			{
				if (i == threadIndex) continue;
				float x = fish[i].Position.X;
				float y = fish[i].Position.Y;
				float dist_x = fishPositionX - x;
				float dist_y = fishPositionY - y;
				
				if (dist_x * dist_x + dist_y * dist_y < FISH_VIEW_RANGE*FISH_VIEW_RANGE)
				{
					groupingPoint.X += x;
					groupingPoint.Y += y;
					n++;
				}
			}
			groupingPoint.X /= n;
			groupingPoint.Y /= n;
			groupingDirection.X = (groupingPoint.X - fishPositionX);
			groupingDirection.Y = (groupingPoint.Y - fishPositionY);
			float length = sqrt(groupingDirection.X*groupingDirection.X + groupingDirection.Y * groupingDirection.Y);
			if (abs(length) > 0.001)
			{
				groupingDirection.X /= length;
				groupingDirection.Y /= length;
			}
		}
		#pragma endregion
		CudaVector antyCrowdingVector = CudaVector(0, 0);
		#pragma region SteerToAvoidCrowdingLocalFlockmates
		if(fish[threadIndex].Settings.grouping)
		{
			for (int i = 0; i < FISH_COUNT; i++)
			{
				float x = fish[i].Position.X;
				float y = fish[i].Position.Y;
				if (abs(x - fish[threadIndex].Position.X) < 0.001 && (y - fish[threadIndex].Position.Y) < 0.001) continue;
				//Vector fromLocalToNeigh = Vector(x - Position.X, y - Position.Y);
				float fromLocalToNeigh_X = x - fish[threadIndex].Position.X;
				float fromLocalToNeigh_Y = y - fish[threadIndex].Position.Y;
				float fromLocalToNeighDist = sqrt((fromLocalToNeigh_X*fromLocalToNeigh_X) + (fromLocalToNeigh_Y*fromLocalToNeigh_Y));
				if (fromLocalToNeighDist <= (FISH_COLISION_RANGE * fish[i].Settings.size + FISH_COLISION_RANGE * fish[threadIndex].Settings.size))
				{
					fromLocalToNeigh_X /= fromLocalToNeighDist;
					fromLocalToNeigh_Y /= fromLocalToNeighDist;
					fromLocalToNeigh_X *= ((FISH_COLISION_RANGE * fish[i].Settings.size + FISH_COLISION_RANGE * fish[threadIndex].Settings.size) - fromLocalToNeighDist);
					fromLocalToNeigh_Y *= ((FISH_COLISION_RANGE * fish[i].Settings.size + FISH_COLISION_RANGE * fish[threadIndex].Settings.size) - fromLocalToNeighDist);
					antyCrowdingVector.X -= fromLocalToNeigh_X;
					antyCrowdingVector.Y -= fromLocalToNeigh_Y;
				}
			}
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

Fish* d_fish;

extern "C" void InitParallerlSteering(Fish* h_fish)
{
	cudaMalloc((void**)&d_fish, FISH_COUNT * sizeof(Fish));
	cudaMemcpy(d_fish, h_fish, FISH_COUNT * sizeof(Fish), cudaMemcpyHostToDevice);
}

extern "C" void FinalizeParallerlSteering()
{
	cudaFree(d_fish);
}

extern "C" void ParallelSteering(Fish* h_fish, float MouseX, float MouseY)
{
	MoveFish << <1 + (FISH_COUNT/1024), 1024 >> > (d_fish, MouseX, MouseY);
	gpuErrchk(cudaPeekAtLastError());
	cudaThreadSynchronize();
	cudaMemcpy(h_fish, d_fish, FISH_COUNT * sizeof(Fish), cudaMemcpyDeviceToHost);
}

