#include"Commen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"f_GaussFilter.h"
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_Commen_MixLayer.h"
#include"f_SoftSkin_DetailsAddingMethod.h"
#include"f_SmartBlur.h"
#include"f_SurfaceBlur.h"

/*************************************************************************
*Function: SOFT SKIN DETAILSADDED METHOD
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*ratio: Intensity of softskin,range [0,100]
*K: intensity of details controled, range [0,300] 
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin_DetailsAddingMethod(unsigned char* srcData, int width, int height, int stride, int ratio, float K)
{
	int ret = 0;
	int len = sizeof(unsigned char) * height * stride;
	unsigned char*coarseSmoothData = (unsigned char*)malloc(len);
	unsigned char*fineSmoothData = (unsigned char*)malloc(len);
	memcpy(coarseSmoothData, srcData, len);
	memcpy(fineSmoothData, srcData, len);
	int std_fine = 5;
	int std_coarse = 10;
	f_SmartBlur(fineSmoothData, width, height, stride, std_fine, 30);
	f_SmartBlur(coarseSmoothData, width, height, stride, std_coarse, 30);
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * len);
	memcpy(skinPDF, coarseSmoothData, sizeof(unsigned char) * len);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	float maskSmoothRadius = 3;
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	unsigned char* pSrc = srcData;
	unsigned char* pCoarse = coarseSmoothData;
	unsigned char* pFine = fineSmoothData;
	unsigned char* pSkin = skinPDF;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int alpha = *pSkin * ratio / 100;
			int detailsFine = pSrc[0] - pFine[0];
			int detailsCoarse = pFine[0] - pCoarse[0];
			float K0 = alpha / 255.0f;
			pSrc[0] = (unsigned char)CLIP3(pCoarse[0] + (255 - alpha) * detailsCoarse / 255 + (1.0f - K0 * K) * detailsFine, 0, 255);

			detailsFine = pSrc[1] - pFine[1];
			detailsCoarse = pFine[1] - pCoarse[1];
			pSrc[1] = (unsigned char)CLIP3(pCoarse[1] + (255 - alpha) * detailsCoarse / 255 + (1.0f - K0 * K) * detailsFine, 0, 255);

			detailsFine = pSrc[2] - pFine[2];
			detailsCoarse = pFine[2] - pCoarse[2];
			pSrc[2] = (unsigned char)CLIP3(pCoarse[2] + (255 - alpha) * detailsCoarse / 255 + (1.0f - K0 * K) * detailsFine, 0, 255);

			pSrc += 4;
			pCoarse += 4;
			pFine += 4;
			pSkin += 4;
		}
	}
	free(skinPDF);
	free(coarseSmoothData);
	free(fineSmoothData);
	return ret;
};