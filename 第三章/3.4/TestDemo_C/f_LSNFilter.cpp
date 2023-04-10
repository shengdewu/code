#include <string.h>
#include <stdlib.h>
#include <math.h>
#include"f_LSNFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

int MeanCovMapCalculate(unsigned char* srcData, int width, int height ,int stride, unsigned long* meanData, unsigned long* covData,int radius)
{
	unsigned long* stdData = (unsigned long*)malloc(sizeof(unsigned long) * width * height);
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
	long sumMean = 0;
	unsigned char* pSrc = srcData;
	int* tempMean = (int*)malloc(sizeof(int)* width);
	memset(tempMean,0,sizeof(int) * width);
	long sumCov = 0;
	int* tempCov = (int*)malloc(sizeof(int)* width);
	memset(tempCov,0,sizeof(int) * width);
	for(k = -radius; k <= radius; k++)
	{
		for(j = 0; j< width; j++)
		{
			t1 = abs(k) * stride;
			tempMean[j] += pSrc[j + t1];
			tempCov[j] += stdData[j + t1];
		}
	}
	for (i = 0; i < height; i++)
    {
        sumMean = 0, sumCov = 0;
        for (j = -radius; j <= radius; j++)
        {
			 t = abs(j);
             sumMean += tempMean[t];    
			 sumCov += tempCov[t];    
        }
        for (j = 0; j < width; j++)
        {
			 t = j + i * stride;
             meanData[t] = (int)(sumMean / winSize);
			 covData[t] = (int)(sumCov / winSize);
			 if (j < width - 1)
			 {
				 t = abs(j - radius);
				 t1 = (j + radius + 1) % width;
			     sumMean = sumMean - tempMean[t] + tempMean[t1];
				 sumCov = sumCov - tempCov[t] + tempCov[t1];
			 }                                
        }
        if (i < height - 1)
        {
             for (k = 0; k < width; k++)
			 {
				 t = k + abs(i - radius) * stride;
				 t1 = k + (i + radius + 1) % height * stride;
			     tempMean[k] = tempMean[k] - pSrc[t] + pSrc[t1];
				 tempCov[k] = tempCov[k] - stdData[t] + stdData[t1];
			 }               
        }
    }
	free(tempMean);
	free(tempCov);
	return 0;
};

int LSNFilterOneChannel(unsigned char* srcData, int width ,int height, int radius, int delta2)
{	
	int len = sizeof(unsigned long) * width * height;
	unsigned char* dstData = (unsigned char*)malloc(len);
	unsigned long* meanData = (unsigned long*) malloc(len);
	unsigned long* covData = (unsigned long*) malloc(len);
	memset(meanData, 0, len);
	memset(covData, 0, len);
    MeanCovMapCalculate(srcData, width, height, width, meanData, covData, radius);
	float mean = 0, cov = 0, K = 0;
	int i, j, num = 2 * radius + 1;
	num = num * num;
	int gray = 0;
	float det = 0;
	for(int i = 0; i < width * height; i++)
	{
		det += (float)covData[i] / (width * height);
	}
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			len = i + j * width;
			mean = (float)meanData[len];
			cov = (float)covData[len];
			cov = cov - mean * mean;
			K = cov / (cov + delta2);
			gray = (int)((1.0 - K) * mean + K * (float)srcData[len]);
			dstData[len] = CLIP3(gray, 0, 255);
		}
	}
	memcpy(srcData, dstData, len);
	free(meanData);
	free(covData);
	free(dstData);
	return 0;
};
/*************************************************************************
*Function: LSNF filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*radius: filter radius,[0,++],default 10
*delta2:  delta*delta,[0,++],default 400
*Return: 0-OK,other failed
**************************************************************************/
int f_LSNFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int delta2)
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
	LSNFilterOneChannel(yData, nWidth, nHeight, radius, delta2);
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

