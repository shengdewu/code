#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"f_AnisotropicFilter.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

static int AnisotropicFilter(unsigned char* srcData, int width ,int height, int stride, int iter, float k, float lambda, int offset)
{
	int ret = 0;
	if(iter <= 0 || k == 0 || lambda == 0 || offset == 0)
		return 1;
	int i, j, pos1, pos2, pos3, pos4, n, pos_src;
	int NI, SI, EI, WI;
	float cN, cS, cE, cW;
	unsigned char* grayData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	if(grayData == NULL)
		return 1;
	unsigned char* pSrc = srcData;	
	float MAP[512];
	float kk = 1.0f / (k * k);
	for(i = -255; i <= 255; i++)
	{
		MAP[i + 255] = exp(- i * i * kk) * lambda * i; 
	}
	int r, g, b;
	for(n = 0; n < iter; n++)
	{
		memcpy(grayData, srcData, sizeof(unsigned char) * height * stride);
		pSrc = srcData;
		for(j = 0; j < height; j++)
		{
			for(i = 0; i < width; i++)
			{
				pos_src = (i << 2) + j * stride;
				pos1 = (i << 2) + CLIP3((j - offset), 0, height - 1) * stride;				
				pos2 = (i << 2) + CLIP3((j + offset), 0, height - 1) * stride;				
				pos3 = (CLIP3((i - offset), 0, width - 1) << 2) + j * stride;			
				pos4 = (CLIP3((i + offset), 0, width - 1) << 2) + j * stride;
				b = grayData[pos_src];
				NI = grayData[pos1] - b;
				SI = grayData[pos2] - b;
				EI = grayData[pos3] - b;
				WI = grayData[pos4] - b;
				cN = MAP[NI + 255];// opt:exp(-NI*NI / (k * k));
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[0] = (int)(CLIP3((b + (cN + cS + cE + cW)), 0, 255));				

				pos_src = pos_src + 1;
				pos1 = pos1 + 1;
				pos2 = pos2 + 1;
				pos3 = pos3 + 1;
				pos4 = pos4 + 1;
				g = grayData[pos_src];
				NI = grayData[pos1] - g;
				SI = grayData[pos2] - g;
				EI = grayData[pos3] - g;
				WI = grayData[pos4] - g;
				cN = MAP[NI + 255];
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[1] = (int)(CLIP3((g + (cN + cS + cE + cW)), 0, 255));

				pos_src = pos_src + 1;
				pos1 = pos1 + 1;
				pos2 = pos2 + 1;
				pos3 = pos3 + 1;
				pos4 = pos4 + 1;
				r = grayData[pos_src];
				NI = grayData[pos1] - r;
				SI = grayData[pos2] - r;
				EI = grayData[pos3] - r;
				WI = grayData[pos4] - r;
				cN = MAP[NI + 255];
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[2] = (int)(CLIP3((r + (cN + cS + cE + cW)), 0, 255));
				pSrc += 4;
			}
		}
	}
	free(grayData);
	return ret;
};//
/*************************************************************************
*Function: Anisotropic filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*iter: iteration num, [0,+],default 7
*k:  coefficient of diffusion, [0,+],default 10
*lambda:cofficient of smoothing, default 0.23
*Return: 0-OK,other failed
**************************************************************************/
int f_AnisotropicFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int iter, float k, float lambda)
{
	if (srcData == NULL)
	{
		return 0;
	}
	return AnisotropicFilter(srcData, nWidth, nHeight, nStride, iter, k, lambda, 1);
}

