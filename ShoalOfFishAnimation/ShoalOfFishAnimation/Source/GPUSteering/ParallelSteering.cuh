
#include "../Fish/Fish.h"


extern "C" void ParallelSteering(Fish* h_fish, float MouseX, float MouseY);
extern "C" void InitParallelSteering(Fish* h_fish);
extern "C" void FinalizeParallelSteering();
