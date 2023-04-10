#include"f_Filter.h"
#include"f_Saturation.h"

int f_ColorFilter(unsigned char *srcData, int width, int height, int stride)
{
	return f_Saturation(srcData, width, height, stride, 41);
};