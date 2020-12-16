
#include "../Fish/Fish.h"

extern "C" void InitParallelSteering();

extern "C" void FinalizeParallelSteering();

extern "C" std::string ParallelSteering(Fish* h_fish, float MouseX, float MouseY);