#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"f_SmartBlur.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

int SmartBlurOneChannel(unsigned char* srcData, int width ,int height, int radius, int threshold)
{	
	int len = sizeof(unsigned long) * width * height;
	int i, j;
	int gray = 0;
	unsigned char* tempData = (unsigned char*) malloc(sizeof(unsigned char) * height * width);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * width);
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			len = i + j * width;
			gray = tempData[len];
			int low = CLIP3(gray - threshold, 0, 255);
			int high = CLIP3(gray + threshold, 0, 255);
			int sum = 0;
			int count = 0;
			for(int n = -radius; n <= radius; n++)
			{
				for(int m = -radius; m <= radius; m++)
				{
					int x = CLIP3(i + m, 0, width - 1);
					int y = CLIP3(j + n, 0, height - 1);
					int pos = x + y * width;					
					gray = tempData[pos];
					if(gray > low && gray < high)
					{
						sum += gray;
					    count++;
					}
				}
			}
			gray = count == 0 ? srcData[len] : sum / count;//sum / MAX2(count, 1);						
			srcData[len] = CLIP3(gray, 0, 255);
		}
	}
	free(tempData);
	return 0;
};
/*************************************************************************
*Function: Smart Blur
*Params:
*srcData:32BGRA image buffer
*nWidth:  width of image
*nHeight: height of image
*nStride: Stride of image
*radius: radius of filter, [0,+]
*threshold: threshold of pixels to count,[0,255]
*Return: 0-OK,other failed
**************************************************************************/
int f_SmartBlur(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int threshold)
{
	int ret = 0;
	if (srcData == NULL)
	{
		return ret;
	}
	if(radius == 0 || threshold == 0)
		return ret;
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
	SmartBlurOneChannel(yData, nWidth, nHeight, radius, threshold);
	SmartBlurOneChannel(cbData, nWidth, nHeight, radius, threshold);
	SmartBlurOneChannel(crData, nWidth, nHeight, radius, threshold);
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
	return ret;
}



