#include"f_Threshold.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"
int f_Threshold(unsigned char *srcData, int width, int height, int stride, int T)
{
	int ret = 0;
	int i, j, gray, offset;
	offset = stride - width * 4;
	unsigned char* pSrc = srcData;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			gray = (pSrc[0] + pSrc[1] + pSrc[2]) / 3;
			gray = gray < T ? 0 : 255;
			pSrc[0] = gray;
			pSrc[1] = gray;
			pSrc[2] = gray;
			pSrc += 4;
		}
		pSrc += offset;
	}
	return ret;
};