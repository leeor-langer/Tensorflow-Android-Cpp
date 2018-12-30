#pragma  once

#include <vector>
#include <string>


#ifdef _WINDOWS
#include "../Computation/Logging.h"
#else
#include "Logging.h"
#endif

extern "C" void InitTensorflowModel(
	const char *graphFileName,
	const std::vector<std::pair<const char *, std::vector<long long>>> &modeInputs,
	const std::vector<const char*> &modelOutputNames,
	int loggerSeverity);
        
extern "C" void RunTensorflowModel(
	const char *graphFileName, 
	const std::vector<std::vector<float>>& inputs,
    std::vector<std::vector<float>> &outputs);
        
 