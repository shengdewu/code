#include"f_USM.h"
#include"f_GaussFilter.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"

int f_USM(unsigned char* srcData,int width, int height,int stride,int radius, int amount, int threshold)
{
	int ret = 0;
    if(radius == 0)
		return ret;
	radius = CLIP3(radius, 0, 100);
	amount = CLIP3(amount, 0,500);
	threshold = CLIP3(threshold, 0,255);
	unsigned char* gaussData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(gaussData, srcData, sizeof(unsigned char) * height * stride);
	f_FastGaussFilter(gaussData, width, height, stride, radius);
	int i, j, r, g, b, offset;
	offset = stride - width * 4;
	amount = amount * 128 / 100;
	unsigned char* pSrc = srcData;
	unsigned char* pDst = gaussData;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	unsigned char* pMask = maskData;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			pMask[0] = abs(pSrc[0] - pDst[0]) < threshold ? 0 : 128;
			pMask[1] = abs(pSrc[1] - pDst[1]) < threshold ? 0 : 128;
			pMask[2] = abs(pSrc[2] - pDst[2]) < threshold ? 0 : 128;
			pDst += 4;
			pSrc += 4;
			pMask += 4;
		}
		pDst += offset;
		pSrc += offset;
		pMask += offset;
	}
	pDst = gaussData;
	pSrc = srcData;
	pMask = maskData;
	f_FastGaussFilter(maskData, width, height, stride, radius);
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			b = pSrc[0] - pDst[0];
			g = pSrc[1] - pDst[1];
			r = pSrc[2] - pDst[2];

			b = (pSrc[0] + ((b * amount) >> 7));
			g = (pSrc[1] + ((g * amount) >> 7));
			r = (pSrc[2] + ((r * amount) >> 7));
			
			b = (b * pMask[0] + pSrc[0] * (128 - pMask[0])) >> 7;
			g = (g * pMask[1] + pSrc[1] * (128 - pMask[1])) >> 7;
			r = (r * pMask[2] + pSrc[2] * (128 - pMask[2])) >> 7;

			pSrc[0] = CLIP3(b, 0, 255);
			pSrc[1] = CLIP3(g, 0, 255);
			pSrc[2] = CLIP3(r, 0, 255);
			pSrc += 4;
			pDst += 4;
			pMask += 4;
		}
		pSrc += offset;
		pDst += offset;
		pMask += offset;
	}
	free(gaussData);
	free(maskData);
	return ret;
};
