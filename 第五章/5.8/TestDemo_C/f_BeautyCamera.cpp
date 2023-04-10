#include"f_BeautyCamera.h"
#include"Commen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"f_GaussFilter.h"
#include"f_LUTFilter.h"
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_Commen_MixLayer.h"
#include"f_LaplaceSharpen.h"
#include"f_MeanFilter.h"

/*************************************************************************
*Function: Smart Blur
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*curveMap: light curve lut, 512x512
*softRatio: intensity of softskin, [0,100]
*skinWhiteRatio: intensity of skin white, [0,100]
*skinColorRatio: intensity of skin color, [-50,50]
*sharpenRatio: intensity of sharpen, [0, 100],default 30
*Return: 0-OK,other failed
**************************************************************************/
int f_BeautyCamera(unsigned char* srcData, int width, int height, int stride, unsigned char* curveMap, int softRatio, int skinWhiteRatio, int skinColorRatio, int sharpenRatio)
{
	//softskin with channel method
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
	ret = f_LUTFilter(curveData, width, height, stride, curveMap);
	float hpRadius = 10.0f * width * height / (594 * 677);
	//ret = f_FastGaussFilter(gaussData, width, height, stride,hpRadius);//此处可以使用均值滤波来代替，提高速度
	ret = f_FastMeanFilter(gaussData, width, height, stride,hpRadius);
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
	//ret = f_FastGaussFilter(skinData, width, height, stride, (float)maskSmoothRadius);//此处可以使用均值滤波来代替，提高速度
	ret = f_FastMeanFilter(skinData, width, height, stride, 2 * maskSmoothRadius);
	if(softRatio != 0)
	{
	    pGauss = skinData;
	    pSrc = srcData;
	    pGreen = greenData;
	    int k = softRatio * 128 / 100;
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
	}
	//skin white
	if(skinWhiteRatio != 0)
	{
		unsigned char lightMap[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 53, 54, 56, 58, 60, 62, 64, 66, 69, 71, 74, 76, 78, 80, 82, 84, 85, 87, 88, 90, 92, 93, 94, 96, 97, 99, 100, 102, 104, 105, 106, 108, 109, 111, 113, 115, 116, 118, 120, 122, 124, 127, 129, 131, 133, 136, 138, 139, 141, 142, 145, 146, 147, 148, 149, 151, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 175, 176, 177, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 190, 191, 192, 193, 194, 195, 196, 196, 197, 198, 199, 200, 200, 201, 202, 203, 203, 204, 205, 206, 206, 207, 208, 209, 209, 210, 211, 211, 212, 213, 213, 214, 215, 215, 216, 217, 217, 218, 218, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225, 225, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 253, 254, 255};
	    pSrc = srcData;
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		pSrc[0] = CLIP3((pSrc[0] * (100 - skinWhiteRatio) + lightMap[pSrc[0]] * skinWhiteRatio) / 100, 0, 255);
	    		pSrc[1] = CLIP3((pSrc[1] * (100 - skinWhiteRatio) + lightMap[pSrc[1]] * skinWhiteRatio) / 100, 0, 255);
	    		pSrc[2] = CLIP3((pSrc[2] * (100 - skinWhiteRatio) + lightMap[pSrc[2]] * skinWhiteRatio) / 100, 0, 255);
	    		pSrc += 4;
	    	}
	    }
	}
	//skin color
	if(skinColorRatio != 0)
	{
		unsigned char coldMap_b[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59, 60, 62, 63, 65, 67, 69, 70, 72, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 90, 93, 95, 96, 97, 98, 99, 100, 101, 104, 105, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 154, 155, 156, 157, 158, 159, 160, 161, 162, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 179, 180, 181, 182, 183, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 211, 211, 212, 212, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 234, 235, 236, 237, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
		unsigned char coldMap_g[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 51, 52, 53, 54, 54, 55, 56, 57, 58, 60, 62, 62, 62, 64, 65, 67, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90, 91, 92, 93, 94, 95, 96, 97, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 178, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 208, 208, 209, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 232, 233, 234, 235, 237, 238, 238, 239, 240, 241, 242, 243, 245, 246, 247, 248, 249, 250, 250, 251, 252, 253, 254, 255};
		unsigned char coldMap_r[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 58, 58, 59, 61, 62, 64, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 87, 88, 89, 90, 89, 90, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 139, 140, 141, 142, 143, 144, 145, 146, 147, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 175, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 204, 205, 205, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 233, 234, 235, 236, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 250, 251, 252, 253, 254, 255};
		
		unsigned char warmMap_g[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90, 91, 92, 93, 93, 94, 95, 97, 98, 99, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 117, 119, 120, 121, 122, 123, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 145, 145, 146, 147, 148, 149, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 174, 174, 175, 176, 177, 178, 179, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 196, 197, 198, 199, 200, 201, 202, 204, 205, 206, 207, 208, 209, 209, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 221, 222, 223, 224, 225, 226, 227, 227, 229, 230, 231, 232, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
		unsigned char warmMap_b[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 58, 59, 60, 61, 61, 63, 64, 65, 64, 65, 66, 67, 68, 69, 70, 71, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 86, 87, 86, 88, 89, 90, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 108, 110, 111, 112, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 133, 133, 134, 135, 136, 137, 138, 138, 139, 140, 141, 142, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 153, 154, 155, 156, 157, 158, 159, 160, 163, 164, 165, 166, 168, 168, 169, 170, 171, 172, 173, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 186, 187, 188, 189, 190, 191, 192, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 206, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 218, 219, 220, 221, 222, 223, 226, 226, 227, 228, 229, 230, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 242, 243, 242, 243, 244, 245, 246, 247, 250, 251, 252, 253, 254, 255};
		unsigned char warmMap_r[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 58, 59, 61, 65, 66, 67, 68, 70, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 86, 87, 88, 89, 90, 91, 92, 93, 94, 96, 97, 98, 99, 100, 102, 103, 104, 106, 107, 108, 110, 111, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 122, 123, 124, 125, 126, 126, 128, 129, 130, 131, 132, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 149, 150, 151, 152, 153, 154, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 181, 181, 182, 183, 182, 183, 184, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 201, 202, 203, 204, 205, 206, 207, 207, 208, 209, 210, 211, 212, 213, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 225, 226, 227, 228, 229, 230, 231, 231, 230, 230, 231, 233, 233, 234, 235, 236, 237, 238, 239, 240, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
		pSrc = srcData;
		if(skinColorRatio < 0)
		{
			for(int j = 0; j < height; j++)
	        {
	        	for(int i = 0; i < width; i++)
	        	{
					int k = -skinColorRatio;
	        		pSrc[0] = CLIP3((pSrc[0] * (50 - k) + coldMap_b[pSrc[0]] * k) / 50, 0, 255);
	        		pSrc[1] = CLIP3((pSrc[1] * (50 - k) + coldMap_g[pSrc[1]] * k) / 50, 0, 255);
	        		pSrc[2] = CLIP3((pSrc[2] * (50 - k) + coldMap_r[pSrc[2]] * k) / 50, 0, 255);
	        		pSrc += 4;
	        		pGreen += 4;
	        	}
	        }
		}
		else
		{
			for(int j = 0; j < height; j++)
	        {
	        	for(int i = 0; i < width; i++)
	        	{
					int k = skinColorRatio;
	        		pSrc[0] = CLIP3((pSrc[0] * (50 - k) + warmMap_b[pSrc[0]] * k) / 50, 0, 255);
	        		pSrc[1] = CLIP3((pSrc[1] * (50 - k) + warmMap_g[pSrc[1]] * k) / 50, 0, 255);
	        		pSrc[2] = CLIP3((pSrc[2] * (50 - k) + warmMap_r[pSrc[2]] * k) / 50, 0, 255);
	        		pSrc += 4;
	        		pGreen += 4;
	        	}
	        }
		}
	}
	//laplace sharpen
	memcpy(greenData, srcData,sizeof(unsigned char) * height * stride);
	f_LaplaceSharpen(greenData, width, height, stride, 0);	
	pSrc = srcData;
	pGreen = greenData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			pSrc[0] = CLIP3((pSrc[0] * (100 - sharpenRatio) + pGreen[0] * sharpenRatio) / 100, 0, 255);
			pSrc[1] = CLIP3((pSrc[1] * (100 - sharpenRatio) + pGreen[1] * sharpenRatio) / 100, 0, 255);
			pSrc[2] = CLIP3((pSrc[2] * (100 - sharpenRatio) + pGreen[2] * sharpenRatio) / 100, 0, 255);
			pSrc += 4;
			pGreen += 4;
		}
	}
	free(gaussData);
	free(greenData);
	free(curveData);
	free(skinData);
	return 0;
};