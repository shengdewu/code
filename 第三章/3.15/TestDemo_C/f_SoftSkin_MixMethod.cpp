#include"Commen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"f_SoftSkin_ChannelMethod.h"
#include"f_GaussFilter.h"
#include"f_LUTFilter.h"
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_SmartBlur.h"
#include"f_SoftSkin_HPMethod.h"
#include"f_SurfaceBlur.h"
#include"f_Commen_MixLayer.h"
#include"f_SoftSkin_MixMethod.h"

int Softskin_A(unsigned char* srcData, int width, int height, int stride, unsigned char* lightMap, int ratio)
{
	int ret = 0;
	unsigned char* greenData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* gaussData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* curveData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* skinData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* smoothData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
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
			t = t * 200 / 255;
			pGreen[0] = CLIP3((pCurve[0] * t + (255 - t) * pSrc[0]) / 255, 0, 255);
			pGreen[1] = CLIP3((pCurve[1] * t + (255 - t) * pSrc[1]) / 255, 0, 255);
			pGreen[2] = CLIP3((pCurve[2] * t + (255 - t) * pSrc[2]) / 255, 0, 255);
			pGreen += 4;
			pGauss += 4;
			pSrc += 4;
			pCurve += 4;
		}
	}
	
	int k = ratio * 128 / 100;
	memcpy(smoothData, greenData, sizeof(unsigned char) * stride * height);
	int smoothRadius = 6;
	int smoothThreshold = 38;
	ret = f_SmartBlur(smoothData, width, height, stride, smoothRadius, smoothThreshold);
	memcpy(skinData, smoothData, sizeof(unsigned char) * height * stride);
	int maskSmoothRadius = 3;
	ret = f_SkinPDF(skinData, width, height, stride);
	ret = f_FastGaussFilter(skinData, width, height, stride, maskSmoothRadius);
	pGauss = skinData;
	pSrc = srcData;
	pGreen = greenData;
	unsigned char* pSmooth = smoothData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int mask = (pGauss[0] + pGauss[1] + pGauss[2]) / 3;
			int tb = CLIP3((pSrc[0] * (255 - mask) + pGreen[0] * mask) / 255, 0, 255);
			int tg = CLIP3((pSrc[1] * (255 - mask) + pGreen[1] * mask) / 255, 0, 255);
			int tr = CLIP3((pSrc[2] * (255 - mask) + pGreen[2] * mask) / 255, 0, 255);
			tb = CLIP3((tb * (255 - mask) + pSmooth[0] * mask) / 255, 0, 255);
			tg = CLIP3((tg * (255 - mask) + pSmooth[1] * mask) / 255, 0, 255);
			tr = CLIP3((tr * (255 - mask) + pSmooth[2] * mask) / 255, 0, 255);
			
			pSrc[0] = CLIP3((pSrc[0] * (128 - k) + tb * k) >> 7, 0, 255);
			pSrc[1] = CLIP3((pSrc[1] * (128 - k) + tg * k) >> 7, 0, 255);
			pSrc[2] = CLIP3((pSrc[2] * (128 - k) + tr * k) >> 7, 0, 255);
			pSrc += 4;
			pGauss += 4;
			pGreen += 4;
			pSmooth += 4;
		}
	}
	free(gaussData);
	free(greenData);
	free(curveData);
	free(skinData);
	free(smoothData);
	return ret;
}
//
//
/*************************************************************************
*Function: SOFT SKIN MIXED METHOD
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*lightMap: light curve map data with format BGRA32
*textureRatio: intensity of details controled, range [0,100] 
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin_MixMethod(unsigned char* srcData, int width, int height, int stride, unsigned char* lightMap, int textureRatio, int ratio)
{
	int ret = 0;
	int length = height * stride;
	unsigned char* smoothData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	unsigned char* hpData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(smoothData, srcData, sizeof(unsigned char) * length);
	memcpy(tempData, srcData, sizeof(unsigned char) * length);
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	int smoothRadius = 8;
	int smoothThreshold = 38;
	int maskSmoothRadius = 3;
	ret = Softskin_A(smoothData, width, height, stride, lightMap, 95);
	memcpy(skinPDF, smoothData, sizeof(unsigned char) * length);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	unsigned char* pSrc = srcData;
	unsigned char* pSkin = skinPDF;
	unsigned char* pSmooth = smoothData;
	unsigned char* pHP = hpData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int mask = 128 - textureRatio * 128 / 100;
			pSrc[0] = CLIP3((pSrc[0] * (128 - mask) + pSmooth[0] * mask) >> 7, 0, 255);
			pSrc[1] = CLIP3((pSrc[1] * (128 - mask) + pSmooth[1] * mask) >> 7, 0, 255);
			pSrc[2] = CLIP3((pSrc[2] * (128 - mask) + pSmooth[2] * mask) >> 7, 0, 255);
			pHP[0] = CLIP3(pSmooth[0] - pSrc[0] + 128, 0, 255);
			pHP[1] = CLIP3(pSmooth[1] - pSrc[1] + 128, 0, 255);
			pHP[2] = CLIP3(pSmooth[2] - pSrc[2] + 128, 0, 255);
			pHP += 4;
			pSmooth += 4;
			pSrc += 4;
			pSkin += 4;
		}
	}
	float hpRadius = 3.1;
	ret = f_FastGaussFilter(hpData, width, height, stride, hpRadius);
	pSrc = srcData;
	pSkin = skinPDF;
	pSmooth = smoothData;
	pHP = hpData;
	int k = ratio * 128 / 100;
	unsigned char* pTemp = tempData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int hpb = pHP[0];
			int hpg = pHP[1];
			int hpr = pHP[2];
			hpb = ModeLinearLight(pSrc[0], hpb);
			hpg = ModeLinearLight(pSrc[1], hpg);
			hpr = ModeLinearLight(pSrc[2], hpr);
			int mask = (pSkin[0] + pSkin[1] + pSkin[2]) / 3;
			hpb = CLIP3((hpb * mask + pSmooth[0] * (255 - mask)) / 255, 0, 255);
			hpg = CLIP3((hpg * mask + pSmooth[1] * (255 - mask)) / 255, 0, 255);
			hpr = CLIP3((hpr * mask + pSmooth[2] * (255 - mask)) / 255, 0, 255);
			
			pSrc[0] = CLIP3((hpb * k + pTemp[0] * (128 - k)) >> 7, 0, 255);
			pSrc[1] = CLIP3((hpg * k + pTemp[1] * (128 - k)) >> 7, 0, 255);
			pSrc[2] = CLIP3((hpr * k + pTemp[2] * (128 - k)) >> 7, 0, 255);
			pSrc += 4;
			pHP += 4;
			pSmooth += 4;
			pSkin += 4;
			pTemp += 4;
		}
	}
	free(skinPDF);
	free(smoothData);
	free(hpData);
	free(tempData);
	return ret;
};