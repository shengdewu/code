#include"Commen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"f_SoftSkin_ChannelMethod.h"
#include"f_GaussFilter.h"
#include"f_LUTFilter.h"
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_Commen_MixLayer.h"

/*************************************************************************
*Function: SOFT SKIN CHANNEL METHOD
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*lightMap: light curve map data with format BGRA32
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin_ChannelMethod(unsigned char* srcData, int width, int height, int stride, unsigned char* lightMap, int ratio)
{
	int ret = 0;
	unsigned char* greenData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* gaussData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* curveData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* skinData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* pSrc = srcData;
	unsigned char* pGreen = greenData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			pGreen[0] = pSrc[0];
			pGreen[1] = pSrc[0];
			pGreen[2] = pSrc[0];
			pSrc += 4;
			pGreen += 4;
		}
	}
	memcpy(gaussData, greenData, sizeof(unsigned char) * height * stride);
	memcpy(curveData, srcData, sizeof(unsigned char) * height * stride);
	ret = f_LUTFilter(curveData, width, height, stride, lightMap);
	float hpRadius = 10.0f * width * height / (594 * 677);
	ret = f_FastGaussFilter(gaussData, width, height, stride,hpRadius);
	pSrc = srcData;
	pGreen = greenData;
	unsigned char* pCurve = curveData;
	unsigned char* pGauss = gaussData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int t;
			t = CLIP3(pGauss[0] - pGreen[0] + 128, 0, 255);
			t = ModeSuperposition(t, t);
			t = ModeSuperposition(t, t);
			t = t * 220 / 255;
			pGreen[0] = CLIP3((pCurve[0] * t + (255 - t) * pSrc[0]) / 255, 0, 255);
			pGreen[1] = CLIP3((pCurve[1] * t + (255 - t) * pSrc[1]) / 255, 0, 255);
			pGreen[2] = CLIP3((pCurve[2] * t + (255 - t) * pSrc[2]) / 255, 0, 255);
			pGreen += 4;
			pGauss += 4;
			pSrc += 4;
			pCurve += 4;
		}
	}
	memcpy(skinData, greenData, sizeof(unsigned char) * height * stride);
	int maskSmoothRadius = 3 * width * height / (594 * 677);
	ret = f_SkinPDF(skinData, width, height, stride);
	ret = f_FastGaussFilter(skinData, width, height, stride, maskSmoothRadius);
	pGauss = skinData;
	pSrc = srcData;
	pGreen = greenData;
	int k = ratio * 128 / 100;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int mask = (pGauss[0] + pGauss[1] + pGauss[2]) / 3;
			int tb = CLIP3((pSrc[0] * (255 - mask) + pGreen[0] * mask) / 255, 0, 255);
			int tg = CLIP3((pSrc[1] * (255 - mask) + pGreen[1] * mask) / 255, 0, 255);
			int tr = CLIP3((pSrc[2] * (255 - mask) + pGreen[2] * mask) / 255, 0, 255);
			pSrc[0] = CLIP3((pSrc[0] * (128 - k) + tb * k) >> 7, 0, 255);
			pSrc[1] = CLIP3((pSrc[1] * (128 - k) + tg * k) >> 7, 0, 255);
			pSrc[2] = CLIP3((pSrc[2] * (128 - k) + tr * k) >> 7, 0, 255);
			pSrc += 4;
			pGauss += 4;
			pGreen += 4;
		}
	}
	free(gaussData);
	free(greenData);
	free(curveData);
	free(skinData);
	return ret;
};

