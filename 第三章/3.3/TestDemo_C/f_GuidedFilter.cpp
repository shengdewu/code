#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include"f_GuidedFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

static int MeanCovMapCalculate(float* srcData, int width, int height, float* meanData, int radius)
{
	float* stdData = (float*)malloc(sizeof(float) * width * height);
	int i, j, k;
	int pos = 0;
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			pos = i + j * width;
			stdData[pos] = srcData[pos] * srcData[pos];
		}
	}
    if(srcData == NULL || meanData == NULL)
	return 1;
	if(radius == 0)
		return 0;
	if(radius > MIN2(width,height) / 2)
		radius = (int)(MIN2(width, height) / 2 - 0.5);
	int t = 0, t1 = 0;
	int block = (radius << 1) + 1;
	int winSize = block * block;
	float sumMean = 0;
	float* pSrc = srcData;
	float* tempMean = (float*)malloc(sizeof(float)* width);
	memset(tempMean,0,sizeof(float) * width);
	for(k = -radius; k <= radius; k++)
	{
		for(j = 0; j< width; j++)
		{
			t1 = abs(k) * width;
			tempMean[j] += pSrc[j + t1];
		}
	}
	for (i = 0; i < height; i++)
    {
        sumMean = 0;
        for (j = -radius; j <= radius; j++)
        {
			 t = abs(j);
             sumMean += tempMean[t];       
        }
        for (j = 0; j < width; j++)
        {
			 t = j + i * width;
             meanData[t] = (float)(sumMean / winSize);
			 if (j < width - 1)
			 {
				 t = abs(j - radius);
				 t1 = (j + radius + 1) % width;
			     sumMean = sumMean - tempMean[t] + tempMean[t1];
			 }                                
        }
        if (i < height - 1)
        {
             for (k = 0; k < width; k++)
			 {
				 t = k + abs(i - radius) * width;
				 t1 = k + (i + radius + 1) % height * width;
			     tempMean[k] = tempMean[k] - pSrc[t] + pSrc[t1];
			 }               
        }
    }
	free(tempMean);
	return 0;
};
//µ¼ÏòÂË²¨
int GuidedFilter(unsigned char* srcData, int width ,int height, int radius, float delta)
{
	float *data = (float*)malloc(sizeof(float) * width * height);
	float *meanIP = (float*)malloc(sizeof(float) * width * height);
	float *corrIP = (float*)malloc(sizeof(float) * width * height);
	float *varIP = (float*)malloc(sizeof(float) * width * height);
	float *a = (float*)malloc(sizeof(float) * width * height);
	float *b = (float*)malloc(sizeof(float) * width * height);
	for(int i = 0; i < width * height; i++)
	{
		data[i] = (float)srcData[i] / 255.0;
	}
	//mean and cov compute
	MeanCovMapCalculate(data, width, height, meanIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		data[i] *= data[i];
	}
	//mean and cov compute
	MeanCovMapCalculate(data, width, height, corrIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		varIP[i] = corrIP[i] - meanIP[i] * meanIP[i];
	}
	for(int i = 0; i < width * height; i++)
	{
		a[i] = varIP[i] / (varIP[i] + delta);
		b[i] = meanIP[i] - a[i] * meanIP[i];
	}
	//mean and cov compute
    MeanCovMapCalculate(a, width, height, meanIP, radius);
	MeanCovMapCalculate(b, width, height, corrIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		srcData[i] = (unsigned char)(CLIP3((meanIP[i] * srcData[i]/255.0f + corrIP[i])*255.0f, 0, 255));
	}
	free(data);
	free(meanIP);
	free(corrIP);
	free(varIP);
	free(a);
	free(b);
	return 0;
};
/*************************************************************************
*Function: Guided filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*radius: filter radius,[0,++],default 10
*delta:  delta,[0,++],default 0.003
*Return: 0-OK,other failed
**************************************************************************/
int f_GuidedFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, float delta)
{
	if (srcData == NULL)
	{
		return 0;
	}
	unsigned char* yData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* cbData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* crData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* pSrc = srcData;
	int Y, CB, CR;
	unsigned char* pY = yData;
	unsigned char* pCb = cbData;
	unsigned char* pCr = crData;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			RGBToYCbCr(pSrc[2],pSrc[1],pSrc[0],&Y,&CB,&CR);
			*pY = Y;
			*pCb = CB;
			*pCr = CR;
			pY++;
			pCb++;
			pCr++;
			pSrc += 4;
		}
	}
	GuidedFilter(yData, nWidth, nHeight, radius, delta);
	pSrc = srcData;
	pY = yData;
	pCb = cbData;
	pCr = crData;
	int R, G, B;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			YCbCrToRGB(*pY, *pCb, *pCr, &R, &G, &B);
            pSrc[0] = B;
			pSrc[1] = G;
			pSrc[2] = R;
			pY++;
			pCb++;
			pCr++;
			pSrc += 4;
		}
	}
	return 0;
}
