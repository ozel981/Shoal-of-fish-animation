#include <iostream>

#include "../AnimationWindow/AnimationWindow.h"


int main()
{
	bool isCPU = true;
	
	std::cout << "1. CPU steereing" << std::endl;
	std::cout << "2. GPU steering" << std::endl;
	std::cout << ":";
	int x;
	scanf("%d",&x);
	if (x == 1) AnimationWindow(isCPU).Run();
	if (x == 2) printf("GPU version not ready!");

	return 0;
}