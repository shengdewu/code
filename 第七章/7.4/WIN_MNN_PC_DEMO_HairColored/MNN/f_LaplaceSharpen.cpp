#include"f_LaplaceSharpen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"

int f_LaplaceSharpen(unsigned char* srcData,int width, int height,int stride,int mode)
{
	int ret = 0;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	int offset = stride - width * 4;
	int pos;
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	if(mode == 0)
	{
	    for(int j = 1; j < height - 1; j++)
	    {
	    	for(int i = 1; i < width - 1; i++)
	    	{
	    		pos = i * 4 + j * stride;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 4 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride]), 0, 255);
	    		pos++;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 4 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride]), 0, 255);
	    		pos++;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 4 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride]), 0, 255);
	    	}
	    }
	}
	else
	{
		for(int j = 1; j < height - 1; j++)
	    {
	    	for(int i = 1; i < width - 1; i++)
	    	{
	    		pos = i * 4 + j * stride;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 8 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride] - tempData[pos - 4 - stride] - tempData[pos + 4 - stride] - tempData[pos - 4 + stride] - tempData[pos + 4 + stride]), 0, 255);
	    		pos++;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 8 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride] - tempData[pos - 4 - stride] - tempData[pos + 4 - stride] - tempData[pos - 4 + stride] - tempData[pos + 4 + stride]), 0, 255);
	    		pos++;
	    		srcData[pos] = CLIP3(tempData[pos] + (tempData[pos] * 8 - tempData[pos - stride] - tempData[pos - 4] - tempData[pos + 4] - tempData[pos + stride] - tempData[pos - 4 - stride] - tempData[pos + 4 - stride] - tempData[pos - 4 + stride] - tempData[pos + 4 + stride]), 0, 255);
	    	}
	    }
	}
	free(tempData);
	return ret;
};