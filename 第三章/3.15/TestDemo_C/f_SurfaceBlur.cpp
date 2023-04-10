#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"f_SurfaceBlur.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

static int SurfaceBlurOneChannel(unsigned char* srcData, int width, int height, float* map, int radius)
{
	int ret = 0;
	int i, j, n, m, len;
	len = sizeof(unsigned char) * width * height;
	unsigned char* tempData = (unsigned char*)malloc(len);
	int* tmap = (int*)malloc(sizeof(int) * height);
	if(tempData == NULL || tmap == NULL)
	{
		ret = 1;
		return ret;
	}
	if(NULL == tempData || NULL == tmap)
		return 1;
	for(i = 0; i < height; i ++)
	{
		tmap[i] = i * width;
	}
	memcpy(tempData, srcData, len);
	int kx, ky;
	len = (radius << 1) + 1;
	int gray = 0;
    float sum, sum_a;
	int pos, pos_a;
    unsigned char val;
    for(j = 0; j < height; j++)
    {
        for(i = 0; i < width; i++)
        {
			pos = i + tmap[j];
            val = tempData[pos];
            sum = 0;
            sum_a = 0;
            for(n = -radius; n <= radius; n++)
            {
				ky = CLIP3(j + n, 0, height - 1);
				pos_a = tmap[ky];
                for(m = -radius; m <= radius; m++)
                {
					kx = CLIP3(i + m, 0, width - 1);
                    gray = tempData[kx + pos_a];
                    sum_a += map[gray - val];
                    sum += gray * map[gray - val];
                }
            }
            gray = sum_a == 0 ? gray : sum / sum_a;//(int)(sum / MAX2(sum_a, 0.1));
            srcData[pos] = gray;//CLIP3(gray, 0 , 255);
        }
    }
	free(tempData);
	free(tmap);
	return ret;
};
/*****************************************************
*Function: Surface blur
*Params:
*srcData-32BGRA image data
*width-width of image
*height-height of image
*stride-Stride of image
*radius-radius of surface blur, [0,100]
*threshold-Threshold of surface blur, [0,255]
*Return: 0-OK, or failed.
******************************************************/
int f_SurfaceBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold)
{
	if (srcData == NULL || radius == 0 || threshold == 0)
	{
		return 0;
	}
	unsigned char* yData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* cbData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* crData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* pSrc = srcData;
	int Y, CB, CR;
	unsigned char* pY = yData;
	unsigned char* pCb = cbData;
	unsigned char* pCr = crData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
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
	float matrixItems[511];//255*2+1
	float* items = &matrixItems[255];
	float fv = threshold * 2.5f;
	for(int i = 0; i < 256; i++)
	{          
	 items[-i] = items[i] = MAX2(1 - i / fv, 0);
	}
	SurfaceBlurOneChannel(yData, width, height, items, radius);
	SurfaceBlurOneChannel(cbData, width, height, items, radius);
	SurfaceBlurOneChannel(crData, width, height, items, radius);
	pSrc = srcData;
	pY = yData;
	pCb = cbData;
	pCr = crData;
	int R, G, B;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
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
