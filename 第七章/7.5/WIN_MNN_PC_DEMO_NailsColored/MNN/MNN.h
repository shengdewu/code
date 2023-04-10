#ifndef __MNN__
#define __MNN__

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





int f_MNN_Initialize(char* modelPath, int tensorSize, int stdMode, int numThread);
void f_MNN_Unitialize();
//int f_MNN_Process(unsigned char* srcData, int width, int height, int stride, unsigned char* dstData, int dWidth, int dHeight, int dStride);
int f_MNN_Process(unsigned char* srcData, int width, int height, int stride);

#endif
