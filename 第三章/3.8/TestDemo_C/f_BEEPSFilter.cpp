#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"f_BEEPSFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

double Gaussian(int u, int v, double sigma)
{
    int t = -(u - v) * (u - v);
    return exp((double)t / sigma);
}
int BEEPSHorizontal(unsigned char* srcPtr, int width, int height, unsigned char* outData, double sigma, int c)
{
	unsigned char* F = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    int *s = (int*)malloc(sizeof(int) * width);
	int *v = (int*)malloc(sizeof(int) * width);
    int pos = 0, X = 0, Y = 0;
    int p = 0;
	memset(F, 0, width * height);
	memset(outData, 0, width * height);
	memset(s, 0, width);
	memset(v, 0, width);
	unsigned char* D = outData;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            X = width - 1 - x;
            Y = height - 1 - y;
            if (x == 0)
            {
                pos = x + y * width;
                F[pos] = srcPtr[pos];
                s[0] = srcPtr[pos];
                p = X;
                pos = p + Y * width;
                v[p] = srcPtr[pos];
                D[pos] = srcPtr[pos];
            }
            else
            {
                p = x;
                pos = p + y * width;
                s[p] = (int)(10.0 * Gaussian(srcPtr[pos], F[pos - 1], sigma));
                F[pos] = CLIP3((((100 - s[p] * c) * srcPtr[pos] + s[p] * c * F[pos - 1]) / 100), 0, 255);
               
                p = X;
                pos = p + Y * width;
                v[p] = (int)(10.0 * Gaussian(srcPtr[pos], D[pos + 1], sigma));
                D[pos] = CLIP3((((100 - v[p] * c) * srcPtr[pos] + v[p] * c * D[pos + 1]) / 100), 0, 255);
            }

        }
    }
    for (int i = 0; i < height * width; i++)
    {
        D[i] = CLIP3(((10 * F[i] - (10 - c) * (srcPtr[i]) + 10 * D[i]) / (10 + c)), 0, 255);
    }
	free(F);
	free(s);
	free(v);
	return 0;
}
int BEEPSVertical(unsigned char* srcPtr, int width, int height, unsigned char* outData, double sigma, int c)
{
	unsigned char* F = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    unsigned char* D = outData;
	int* s = (int*)malloc(sizeof(int) * height);
	int* v = (int*)malloc(sizeof(int) * height);
    int pos = 0, X = 0, Y = 0;
	memset(s, 0, height);
	memset(v, 0, height);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            X = width - 1 - x;
            Y = height - 1 - y;
            if (y == 0)
            {
                pos = x + y * width;
                F[pos] = srcPtr[pos];
                s[y] = srcPtr[pos];

                pos = X + Y * width;
                D[pos] = srcPtr[pos];
                v[Y] = srcPtr[pos];
            }
            else
            {
                pos = x + y * width;
                s[y] = (int)(10.0 * Gaussian(srcPtr[pos], F[pos - width], sigma));
                F[pos] = CLIP3((((100 - s[y] * c) * srcPtr[pos] + s[y] * c * F[pos - width]) / 100), 0, 255);


                pos = X + Y * width;
                v[Y] = (int)(10.0 * Gaussian(srcPtr[pos], D[pos + width], sigma));
                D[pos] = CLIP3((((100 - v[Y] * c) * srcPtr[pos] + v[Y] * c * D[pos + width]) / 100), 0, 255);
            }

        }
    }
    for (int i = 0; i < height*width; i++)
    {
        D[i] = CLIP3(((10 * F[i] - (10 - c) * (srcPtr[i]) + 10 * D[i]) / (10 + c)), 0, 255);
    }
    free(F);
	free(s);
	free(v);
	return 0;
}
void BEEPSProcess(unsigned char* srcPtr, int width, int height, float sigma,float c)
{
	float* GMAP = (float*)malloc(sizeof(float) * 256 * 256);
	for(int j = 0; j < 256; j++)
	{
		for(int i = 0; i < 256; i++)
		{
			GMAP[i + j * 256] = Gaussian(i, j, sigma);
		}
	}
    sigma = sigma > 50 ? 50 : sigma;
	sigma = sigma * sigma * 2.0f;
	float Lamba =10.0f * (float)( 1 - (sqrt(2.0f * c * c + 1) - 1) / (c * c)); 
    unsigned char* pSrc = srcPtr;
	unsigned char* hValue = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* vValue = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    unsigned char* dstValue = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    BEEPSHorizontal(pSrc, width, height,hValue, sigma, Lamba);
	BEEPSVertical(hValue, width, height, vValue, sigma, Lamba);
    BEEPSVertical(pSrc, width, height,hValue, sigma, Lamba);
	BEEPSHorizontal(hValue, width, height,dstValue, sigma, Lamba);
    for (int i = 0; i < width * height; i++)
    {
        *pSrc++ = CLIP3(((vValue[i] + dstValue[i]) / 2), 0, 255);
    }
	free(hValue);
	free(vValue);
	free(dstValue);
}


/*************************************************************************
*Function: BEEPS Filter
*Params:
*srcData:32BGRA image buffer
*nWidth:  width of image
*nHeight: height of image
*nStride: Stride of image
*radius: radius of filter, [0,+]
*delta: standard deviation of gaussian,[0,+]
*delta_s: spatial contraDecay of lambda,[0,+]
*Return: 0-OK,other failed
**************************************************************************/
int f_BeepsFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, float delta, float delta_s)
{
	if (srcData == NULL)
	{
		return 0;
	}
	if(delta == 0 || delta_s == 0)
		return 0;
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
	BEEPSProcess(yData, nWidth, nHeight, delta, delta_s);
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
	free(yData);
	free(cbData);
	free(crData);
	return 0;
}



