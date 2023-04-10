#ifndef __MNN_FACEALIGNMENT__
#define __MNN_FACEALIGNMENT__

#include "stdafx.h"
#include"Commen.h"
#include"MNN.h"
#include "ImageProcess.hpp"
#include "Interpreter.hpp"
#define MNN_OPEN_TIME_TRACE
#include <algorithm>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <vector>
#include "AutoTime.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION





int f_MNN_FA_Initialize(char* modelPath, int tensorSize, int stdMode, int numThread);
void f_MNN_FA_Unitialize();
int MNN_FA_Process(unsigned char* srcData, int width, int height, int stride, float facePoints[202]);

#endif
