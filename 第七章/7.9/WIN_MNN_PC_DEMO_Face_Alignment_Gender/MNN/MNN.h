#ifndef __MNN__
#define __MNN__

#include "stdafx.h"
#include"Commen.h"
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





int f_MNN_Initialize(char* fdmodelPath, char* famodelPath, int tensorSize, int stdMode, int numThread);
void f_MNN_Unitialize();
int f_MNN_Process(unsigned char* srcData, int width, int height, int stride);
int f_MNN_FA_Process(unsigned char* srcData, int width, int height, int stride, float facePoints[202]);
#endif
