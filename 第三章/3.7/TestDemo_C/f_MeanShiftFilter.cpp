#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"f_MeanShiftFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

int MeanShiftOneChannel(unsigned char* srcData, int width ,int height, int radius, int threshold, int maxIter)
{	
	int len = sizeof(unsigned long) * width * height;
	int i, j;
	int gray = 0, sum = 0, srcGray = 0, count = 0;
	unsigned char* tempData = (unsigned char*) malloc(sizeof(unsigned char) * height * width);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * width);
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			len = i + j * width;
			int nIter = 0, cx = 0, cy = 0, sumx = 0, sumy = 0;
			srcGray = tempData[len];
			cx = i;
			cy = j;

			while(nIter < maxIter)
			{
				sum = 0;
				sumx = 0;
				sumy = 0;
				count = 0;
				for(int y = cy - radius; y <= cy + radius; y++)
				{
					for(int x = cx - radius; x <= cx + radius; x++)
					{
						int px = CLIP3(x, 0, width - 1);
						int py = CLIP3(y, 0, height - 1);
						len = px + py * width;
						gray = tempData[len];
						if(abs(gray - srcGray) < threshold)
						{
							count++;
							sum += gray;
							sumx += x;
							sumy += y;
						}
					}
				}
				if(count == 0)
					break;
				srcGray = sum / count;
				cx = sumx / count;
				cy = sumy / count;
				nIter++;
			}
			srcData[i + j * width] = srcGray;
		}
	}
	free(tempData);
	return 0;
};
/*************************************************************************
*Function: MeanShift Filter
*Params:
*srcData:32BGRA image buffer
*nWidth:  width of image
*nHeight: height of image
*nStride: Stride of image
*radius: radius of filter, [0,+]
*threshold: threshold of pixels to count,[0,255]
*maxIter: max iteration number of filter
*Return: 0-OK,other failed
**************************************************************************/
int f_MeanShiftFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int threshold, int maxIter)
{
	if (srcData == NULL)
	{
		return 0;
	}
	if(radius == 0 || threshold == 0)
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
	MeanShiftOneChannel(yData, nWidth, nHeight, radius, threshold, maxIter);
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
}


