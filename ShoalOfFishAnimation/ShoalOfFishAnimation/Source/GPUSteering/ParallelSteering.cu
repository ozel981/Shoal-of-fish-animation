#include <iostream>

#include <GL/glew.h>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include "ParallelSteering.cuh"
#include "../Constant/Costant.h"

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

__global__ void MoveFish(Fish *fish)
{
	int threadIndex = threadIdx.x;
	if (threadIndex < FISH_COUNT)
	{
		#pragma region SteerToTheAverageHeadingOfLocalFlockmates

		#pragma endregion

		#pragma region SteerToTheAveragePositionOfLocalFlockmates
		float voctorToAvgPosition_X = 0, voctorToAvgPosition_Y = 0;
		{
			float avgPosition_X = fish[threadIndex].Position.X;
			float avgPosition_Y = fish[threadIndex].Position.Y;
			int n = 1;
			for (int i = 0; i < FISH_COUNT; i++)
			{
				if (i == threadIndex) continue;
				float x = fish[i].Position.X;
				float y = fish[i].Position.Y;
				float dist_x = fish[threadIndex].Position.X - x;
				float dist_y = fish[threadIndex].Position.Y - y;
				
				if (sqrt(dist_x * dist_x + dist_y * dist_y) < FISH_VIEW_RANGE)
				{
					avgPosition_X += x;
					avgPosition_Y += y;
					n++;
				}
			}
			avgPosition_X /= n;
			avgPosition_Y /= n;
			voctorToAvgPosition_X = (avgPosition_X - fish[threadIndex].Position.X);
			voctorToAvgPosition_Y = (avgPosition_Y - fish[threadIndex].Position.Y);
			float length = sqrt(voctorToAvgPosition_X*voctorToAvgPosition_X + voctorToAvgPosition_Y * voctorToAvgPosition_Y);
			if (abs(length) > 0.001)
			{
				voctorToAvgPosition_X /= length;
				voctorToAvgPosition_Y /= length;
			}
		}
		#pragma endregion
		
		#pragma region SteerToAvoidCrowdingLocalFlockmates
		float resultantVersor_X = 0, resultantVersor_Y = 0;
		for (int i = 0; i < FISH_COUNT; i++)
		{
			float x = fish[i].Position.X;
			float y = fish[i].Position.Y;
			if (abs(x - fish[threadIndex].Position.X) < 0.001 && (y - fish[threadIndex].Position.Y) < 0.001) continue;
			//Vector fromLocalToNeigh = Vector(x - Position.X, y - Position.Y);
			float fromLocalToNeigh_X = x - fish[threadIndex].Position.X;
			float fromLocalToNeigh_Y = y - fish[threadIndex].Position.Y;
			float fromLocalToNeighDist = sqrt((fromLocalToNeigh_X*fromLocalToNeigh_X) + (fromLocalToNeigh_Y*fromLocalToNeigh_Y));
			if (fromLocalToNeighDist <= (FISH_COLISION_RANGE * 2))
			{
				fromLocalToNeigh_X /= fromLocalToNeighDist;
				fromLocalToNeigh_Y /= fromLocalToNeighDist;
				fromLocalToNeigh_X *= ((FISH_COLISION_RANGE * 2) - fromLocalToNeighDist);
				fromLocalToNeigh_Y *= ((FISH_COLISION_RANGE * 2) - fromLocalToNeighDist);
				resultantVersor_X -= fromLocalToNeigh_X;
				resultantVersor_Y -= fromLocalToNeigh_Y;
			}
		}
		#pragma endregion

		__syncthreads();
		fish[threadIndex].Position.X += voctorToAvgPosition_X;		
		fish[threadIndex].Position.Y += voctorToAvgPosition_Y;
		fish[threadIndex].Position.X += resultantVersor_X*0.6;
		fish[threadIndex].Position.Y += resultantVersor_Y*0.6;
	}

}

__global__ void FishM(float* x, float* y)
{
	int threadIndex = threadIdx.x;
	if (threadIndex < FISH_COUNT)
	{
		float a_x = x[threadIndex];
		float a_y = y[threadIndex];
		int n = 1;
		for (int i = 0; i < FISH_COUNT; i++)
		{
			if (i == threadIndex) continue;
			float d_x = x[i] - x[threadIndex];
			float d_y = y[i] - y[threadIndex];
			if ((d_x*d_x + d_y * d_y) < FISH_VIEW_RANGE*FISH_VIEW_RANGE)
			{
				a_x += x[i];
				a_y += y[i];
				n++;
			}
		}
		a_x = a_x / n;
		a_y = a_y / n;
		x[threadIndex] = a_x;
		y[threadIndex] = a_y;
	}
}

extern "C" void ParallelSteering(Fish* h_fish, int count)
{
	/*float h_PosX[FISH_COUNT];
	float h_PosY[FISH_COUNT];
	for (int i = 0; i < FISH_COUNT; i++)
	{
		h_PosX[i] = h_fish[i].Position.X;
		h_PosY[i] = h_fish[i].Position.Y;
	}
	float* d_PosX;
	float* d_PosY;
	cudaMalloc((void**)&d_PosX, FISH_COUNT * sizeof(float));
	cudaMalloc((void**)&d_PosY, FISH_COUNT * sizeof(float));
	cudaMemcpy(d_PosX, h_PosX, FISH_COUNT * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_PosY, h_PosY, FISH_COUNT * sizeof(float), cudaMemcpyHostToDevice);
	FishM << <1, FISH_COUNT >> > (d_PosX, d_PosY);
	cudaMemcpy(h_PosX, d_PosX, FISH_COUNT * sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_PosY, d_PosY, FISH_COUNT * sizeof(float), cudaMemcpyDeviceToHost);
	cudaFree(d_PosX);
	cudaFree(d_PosY);
	for (int i = 0; i < FISH_COUNT; i++)
	{
		//h_fish[i].Position.X += h_PosX[i];
		//h_fish[i].Position.Y += h_PosY[i];
		printf("%d:po: %f - %f\n",i, h_PosX[i], h_PosY[i]);

	}*/
	Fish* d_fish;
	cudaMalloc((void**)&d_fish, count * sizeof(Fish));
	cudaMemcpy(d_fish, h_fish, count * sizeof(Fish), cudaMemcpyHostToDevice);
	MoveFish << <1, count >> > (d_fish);
	cudaThreadSynchronize();
	cudaMemcpy(h_fish, d_fish, count * sizeof(Fish), cudaMemcpyDeviceToHost);
	cudaFree(d_fish);
}

