#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_LUTFilter.h"
#include"f_SkinPDF.h"
#include"f_GaussFilter.h"
#include"f_SkinWhite.h"
/*************************************************************************
*Function: SKIN WHITE
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*skinMask: skin mask
*lutData: 32BGRA buffer of lut image.
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_SkinWhite(unsigned char* srcData, int width, int height, int stride, unsigned char* lutData, int ratio)
{
	int ret = 0;
	int length = height * stride;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(tempData, srcData, sizeof(unsigned char) * length);
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(skinPDF, srcData, sizeof(unsigned char) * length);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	int maskSmoothRadius = 3;
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	ret = f_LUTFilter(tempData, width, height, stride, lutData);
	unsigned char* pSrc = srcData;
	unsigned char* pLut = tempData;
	unsigned char* pSkin = skinPDF;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int r, g, b, a;
			b = CLIP3((pSrc[0] * (100 - ratio) + pLut[0] * ratio) / 100, 0, 255);
			g = CLIP3((pSrc[1] * (100 - ratio) + pLut[1] * ratio) / 100, 0, 255);
			r = CLIP3((pSrc[2] * (100 - ratio) + pLut[2] * ratio) / 100, 0, 255);
			a = (pSkin[0] + pSkin[1] + pSkin[2]) / 3;
			pSrc[0] = CLIP3((b * a + pSrc[0] * (255 - a)) / 255, 0, 255);
			pSrc[1] = CLIP3((g * a + pSrc[1] * (255 - a)) / 255, 0, 255);
			pSrc[2] = CLIP3((r * a + pSrc[2] * (255 - a)) / 255, 0, 255);
			pSrc += 4;
			pLut += 4;
			pSkin += 4;
		}
	}
	free(tempData);
	free(skinPDF);
	return ret;
};
/*************************************************************************
*Function: SKIN WHITE USING CURVE 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*belta: intensity of curve enhancement,range[2,10],default:2
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
*Reference: "A Two-Stage Contrast Enhancement Algorithm for Digital Images"
**************************************************************************/
int f_SkinWhiteCurve(unsigned char* srcData, int width, int height, int stride, int belta, int ratio)
{
	int ret = 0;
	int length = height * stride;
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(skinPDF, srcData, sizeof(unsigned char) * length);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	int maskSmoothRadius = 3;
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	unsigned char* pSrc = srcData;
	unsigned char* pSkin = skinPDF;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int r, g, b, a;
			//skin white curve
            b = CLIP3(log((float)pSrc[0] * (belta - 1) / 255.0f + 1) / log((float)belta) * 255.0f, 0, 255);
			g = CLIP3(log((float)pSrc[1] * (belta - 1) / 255.0f + 1) / log((float)belta) * 255.0f, 0, 255);
			r = CLIP3(log((float)pSrc[2] * (belta - 1) / 255.0f + 1) / log((float)belta) * 255.0f, 0, 255);
			b = CLIP3((b * ratio + pSrc[0] * (100 - ratio)) / 100, 0, 255);
			g = CLIP3((g * ratio + pSrc[1] * (100 - ratio)) / 100, 0, 255);
			r = CLIP3((r * ratio + pSrc[2] * (100 - ratio)) / 100, 0, 255);
			//skin pdf
			a = (pSkin[0] + pSkin[1] + pSkin[2]) / 3;
			pSrc[0] = CLIP3((b * a + pSrc[0] * (255 - a)) / 255, 0, 255);
			pSrc[1] = CLIP3((g * a + pSrc[1] * (255 - a)) / 255, 0, 255);
			pSrc[2] = CLIP3((r * a + pSrc[2] * (255 - a)) / 255, 0, 255);
			pSrc += 4;
			pSkin += 4;
		}
	}
	free(skinPDF);
	return ret;
}
 inline int ModeSmoothLight(int basePixel,int mixPixel)
{
  int res = 0;
  res = mixPixel > 128 ? ((int)((float)basePixel+((float)mixPixel+(float)mixPixel-255.0f)*((sqrt((float)basePixel/255.0f) )*255.0f-(float)basePixel)/255.0f)):
		((int)((float)basePixel+((float)mixPixel+(float)mixPixel-255.0f)*((float)basePixel-(float)basePixel*(float)basePixel/255.0f)/255.0f));
  return CLIP3(res, 0, 255);
};
 /*************************************************************************
*Function: SKIN WHITE USING SMOOTH LIGHT OF PS 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_SkinWhitePS(unsigned char* srcData, int width, int height, int stride, int ratio)
{
	int ret = 0;
	int length = height * stride;
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(skinPDF, srcData, sizeof(unsigned char) * length);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	int maskSmoothRadius = 3;
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	unsigned char* pSrc = srcData;
	unsigned char* pSkin = skinPDF;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int r, g, b, a;
			//skin white using smoothlight of ps
            b = ModeSmoothLight(pSrc[0], 255);
			g = ModeSmoothLight(pSrc[1], 255);
			r = ModeSmoothLight(pSrc[2], 255);
			b = CLIP3((b * ratio + pSrc[0] * (100 - ratio)) / 100, 0, 255);
			g = CLIP3((g * ratio + pSrc[1] * (100 - ratio)) / 100, 0, 255);
			r = CLIP3((r * ratio + pSrc[2] * (100 - ratio)) / 100, 0, 255);
			//skin pdf
			a = (pSkin[0] + pSkin[1] + pSkin[2]) / 3;
			pSrc[0] = CLIP3((b * a + pSrc[0] * (255 - a)) / 255, 0, 255);
			pSrc[1] = CLIP3((g * a + pSrc[1] * (255 - a)) / 255, 0, 255);
			pSrc[2] = CLIP3((r * a + pSrc[2] * (255 - a)) / 255, 0, 255);
			pSrc += 4;
			pSkin += 4;
		}
	}
	free(skinPDF);
	return ret;
}