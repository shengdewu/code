
#ifndef __GENDER_MNN__
#define __GENDER_MNN__

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





int f_Gender_MNN_Initialize(char* genderModelPath, int stdMode, int numThread);
void f_Gender_MNN_Unitialize();
int f_Gender_MNN_Process(unsigned char* srcData, int width, int height, int stride, float* outGender);
#endif
