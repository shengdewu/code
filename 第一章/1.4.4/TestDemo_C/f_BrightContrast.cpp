#include"f_BrightContrast.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"
int f_BrightContrast(unsigned char *srcData, int width, int height, int stride, int bright, int contrast)
{
	int ret = 0;
	bright = CLIP3(bright,-100,100);
	contrast = CLIP3(contrast, -100,100);
	//compute average light of image
	int Average = 0;
	int offset = stride - width * 4;
	unsigned char* pSrc = srcData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			Average += (299 *  pSrc[2] + 587 * pSrc[1] + 114 * pSrc[0]) / 1000;
			pSrc += 4;
		}
		pSrc += offset;
	}
	Average = Average / (width * height);
	pSrc = srcData;
	unsigned char BC_MAP[256];
	int temp =0;
	for(int i = 0; i < 256; i++)
	{
		int temp = contrast > 0 ? CLIP3(i + bright, 0, 255) : i;
		if(contrast > 0)
		{
			temp = CLIP3(i + bright, 0, 255);
			temp = CLIP3(Average + (temp - Average) * (1.0f / (1.0f - contrast / 100.0f)), 0, 255);
		}
		else
		{
			temp = i;
			temp = CLIP3(Average + (temp - Average) * (1.0f + contrast / 100.0f), 0, 255);
			temp = CLIP3(temp + bright, 0, 255);
		}
		BC_MAP[i] = temp;
	}
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			pSrc[0] = BC_MAP[pSrc[0]];
			pSrc[1] = BC_MAP[pSrc[1]];
			pSrc[2] = BC_MAP[pSrc[2]];
			pSrc += 4;
		}
		pSrc += offset;
	}
	return ret;
};

