#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_BeautyIris.h"
#include"TRGB2YUV.h"
#include"f_MeanFilter.h"

//void RGB2HSB(unsigned char r, unsigned char g, unsigned char b, float *H, float *S, float* B)
//{
//	int max = MAX2(r, MAX2(g, b));
//	int min = MIN2(r, MIN2(g, b));
//	int temp[3] = {min, r + g + b - max - min, max};
//	int maxIndex = 0, minIndex = 0;
//	int rgb[3] = {r, g, b};
//	for(int i=0;i<3;i++) {
//        if(temp[0]==rgb[i]) minIndex=i;
//        if(temp[2]==rgb[i]) maxIndex=i;
//    }
//	*B = temp[2] / 255.0f;
//	*S = 1.0f - (float)temp[0] / temp[2];
//	*H = maxIndex * 120.0f + 60.0f * (temp[1] / *S / temp[2] + (1.0f - 1.0f / *S)) * ((maxIndex - minIndex + 3) % 3 == 1 ? 1 : -1);
//	*H = (int)(*H + 360) % 360;
//}
//void HSB2RGB(float H, float S, float B, unsigned char* r, unsigned char* g, unsigned char* b)
//{
//	float rgb[3] = {0};
//	for(int offset = 240, i = 0; i < 3; i++, offset -= 120)
//	{
//		float x = abs((int)(H + offset) % 360 - 240);
//		if(x <= 60) rgb[i] = 255;
//		else if(x > 60 && x < 120) rgb[i] = ((1.0f - (x - 60) / 60.0f) * 255.0f);
//		else rgb[i] = 0;
//	}
//	rgb[0] += (255 - rgb[0]) * (1.0f - S);
//	rgb[1] += (255 - rgb[1]) * (1.0f - S);
//	rgb[2] += (255 - rgb[2]) * (1.0f - S);
//
//	rgb[0] *= B;
//	rgb[1] *= B;
//	rgb[2] *= B;
//	*r = CLIP3(rgb[0], 0, 255);
//	*g = CLIP3(rgb[1], 0, 255);
//	*b = CLIP3(rgb[2], 0, 255);
//}

bool f_PNPoly_OPT(int xPoint[],int yPoint[],int pointNum,int x, int y, int minX, int minY, int maxX, int maxY)
{
	if(x < minX || x > maxX || y < minY || y > maxY)
		return false;
	int i, j, c = 0;
	for(i = 0, j = pointNum - 1; i < pointNum; j = i++)
	{
		if((float)((yPoint[i] > y) != (yPoint[j] > y)) && (x < (float)(xPoint[j] - xPoint[i]) * (y - yPoint[i]) / (yPoint[j] - yPoint[i]) + xPoint[i]))
			c = !c;
	}
	return c;
}
/************************************************************
*Function:  f_PNPoly
*Description: Point in the poly or not
*Params:    
*xPoint:  x position or several points input.   
*yPoint:  y position or several points input.   
*pointNum :number of points input.
*(x,y): the current position.
*Return :true or false.    
************************************************************/
bool f_PNPoly(int xPoint[],int yPoint[],int pointNum,int x, int y)
{
	int i, j, c = 0;
	for(i = 0, j = pointNum - 1; i < pointNum; j = i++)
	{
		if((float)((yPoint[i] > y) != (yPoint[j] > y)) && (x < (float)(xPoint[j] - xPoint[i]) * (y - yPoint[i]) / (yPoint[j] - yPoint[i]) + xPoint[i]))
			c = !c;
	}
	return c;
}
/************************************************************
*Function:  Beauty Iris
*Description: Beauty Iris process
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*srcPoints:  13 points of 101 face points.
*maskData:   iris mask data
*mWidth:  width of mask.
*mHeight: height of mask.
*mStride: stride of mask.
maskPoints: center point of mask.
*ratio:  intensity of beauty iris, [0, 100].
*Return :0-OK,or failed    
************************************************************/
int f_BeautyIris(unsigned char* srcData, int width, int height, int stride, int srcPoints[], unsigned char* maskData, int mWidth, int mHeight, int mStride, int maskPoints[], int ratio)
{
	int ret = 0;
	unsigned char* pSrc = srcData;
	int cenx = srcPoints[2 * 12];
	int ceny = srcPoints[2 * 12 + 1];
	//compute iris radius.
	int eyedistance = sqrt((float)(srcPoints[2 * 0] - srcPoints[2 * 6]) * (srcPoints[2 * 0] - srcPoints[2 * 6]) + (srcPoints[2 * 0 + 1] - srcPoints[2 * 6 + 1]) * (srcPoints[2 * 0 + 1] - srcPoints[2 * 6 + 1])) / 2;
	int irisRadius = eyedistance / 2;
	//compute the rectangle of eye area.
	int px = CLIP3(cenx - irisRadius, 0, width - 1);
	int py = CLIP3(ceny - irisRadius, 0, height - 1);
	int pmaxX = CLIP3(cenx + irisRadius, 0, width - 1);
	int pmaxY = CLIP3(ceny + irisRadius, 0, height - 1);
	int w = pmaxX - px;
	int h = pmaxY - py;
	const int pointNum = 12;
	int xPoints[pointNum];
	int yPoints[pointNum];
	int maxX = 0, minX = 100000, maxY = 0, minY = 100000;
	for(int i = 0; i < pointNum; i++)
	{
		xPoints[i] = srcPoints[2 * i];
		yPoints[i] = srcPoints[2 * i + 1];
		maxX = MAX2(srcPoints[2 * i], maxX);
		maxY = MAX2(srcPoints[2 * i + 1], maxY);
		minX = MIN2(srcPoints[2 * i], minX);
		minY = MIN2(srcPoints[2 * i + 1], minY);
	}
	//irisMask feather process
    unsigned char* irisMask = (unsigned char*)malloc(sizeof(unsigned char) * w * h * 4);
	unsigned char* pMask = irisMask;
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			int dis = sqrt((float)(i - irisRadius) * (i - irisRadius) + (j - irisRadius) * (j - irisRadius));
			if(f_PNPoly_OPT(xPoints, yPoints, pointNum, i + px, j + py, minX, minY, maxX, maxY) && dis < irisRadius-3)
				pMask[0] = pMask[1] = pMask[2] = 255;
			else
				pMask[0] = pMask[1] = pMask[2] = 0;
			pMask += 4;
		}
	}
	f_FastMeanFilter(irisMask, w, h, w * 4, 3);
	pMask = irisMask;
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if(pMask[0] > 128)
				pMask[0] = pMask[1] = pMask[2] = 255;
			else
				pMask[0] = pMask[1] = pMask[2] = 0;
			pMask += 4;
		}
	}
	f_FastMeanFilter(irisMask, w, h, w * 4, 3);
	//beauty iris process
	pMask = irisMask;
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if((pMask[0] + pMask[1] + pMask[2]) / 3 > 0)
			{
			    int pos_src = (px + i) * 4 + (py + j) * stride;
			    int mx = i * mWidth / w;
			    int my = j * mHeight / h;
			    int pos_mask = mx * 4 + my * mStride;
			    int r = srcData[pos_src + 2];
			    int g = srcData[pos_src + 1];
			    int b = srcData[pos_src];
			    int mr = maskData[pos_mask + 2];
			    int mg = maskData[pos_mask + 1];
			    int mb = maskData[pos_mask];
			    int Y, U, V, mY, mU, mV;
				//YUV
			    RGB2YUV(r, g, b,&Y, &U, &V);
			    RGB2YUV(mr, mg, mb, &mY, &mU, &mV);
			    unsigned char R, G, B;
			    YUV2RGB(Y, mU, mV, &R, &G, &B);
				//feather mask process
				int a = (pMask[0] + pMask[1] + pMask[2]) / 3;
				B = CLIP3((b * (255 - a) + B * a) >> 8, 0, 255);
				G = CLIP3((g * (255 - a) + G * a) >> 8, 0, 255);
				R = CLIP3((r * (255 - a) + R * a) >> 8, 0, 255);
				//YUV color correct
			    a = CLIP3((r + g + b) / 3 * 2, 0, 255);
				R = (R * a + r * (255 - a)) / 255;
				G = (G * a + g * (255 - a)) / 255;
				B = (B * a + b * (255 - a)) / 255;
				//ratio blending
				B = CLIP3((b * (100 - ratio) + B * ratio) / 100, 0, 255);
				G = CLIP3((g * (100 - ratio) + G * ratio) / 100, 0, 255);
				R = CLIP3((r * (100 - ratio) + R * ratio) / 100, 0, 255);

				srcData[pos_src]     = B;
				srcData[pos_src + 1] = G;
				srcData[pos_src + 2] = R;
			}
			pMask += 4;
		}
	}
	free(irisMask);
	return ret;
};

//int f_BeautyIris(unsigned char* srcData, int width, int height, int stride, int srcPoints[], unsigned char* maskData, int mWidth, int mHeight, int mStride, int maskPoints[], int ratio)
//{
//	int ret = 0;
//	unsigned char* pSrc = srcData;
//	int cenx = srcPoints[2 * 12];
//	int ceny = srcPoints[2 * 12 + 1];
//	//compute iris radius.
//	int eyedistance = sqrt((float)(srcPoints[2 * 0] - srcPoints[2 * 6]) * (srcPoints[2 * 0] - srcPoints[2 * 6]) + (srcPoints[2 * 0 + 1] - srcPoints[2 * 6 + 1]) * (srcPoints[2 * 0 + 1] - srcPoints[2 * 6 + 1])) / 2;
//	int irisRadius = eyedistance / 2;
//	//compute the rectangle of eye area.
//	int px = CLIP3(cenx - irisRadius, 0, width - 1);
//	int py = CLIP3(ceny - irisRadius, 0, height - 1);
//	int pmaxX = CLIP3(cenx + irisRadius, 0, width - 1);
//	int pmaxY = CLIP3(ceny + irisRadius, 0, height - 1);
//	int w = pmaxX - px;
//	int h = pmaxY - py;
//	const int pointNum = 12;
//	int xPoints[pointNum];
//	int yPoints[pointNum];
//	int maxX = 0, minX = 100000, maxY = 0, minY = 100000;
//	for(int i = 0; i < pointNum; i++)
//	{
//		xPoints[i] = srcPoints[2 * i];
//		yPoints[i] = srcPoints[2 * i + 1];
//		maxX = MAX2(srcPoints[2 * i], maxX);
//		maxY = MAX2(srcPoints[2 * i + 1], maxY);
//		minX = MIN2(srcPoints[2 * i], minX);
//		minY = MIN2(srcPoints[2 * i + 1], minY);
//	}
//	//irisMask feather process
//    unsigned char* irisMask = (unsigned char*)malloc(sizeof(unsigned char) * w * h * 4);
//	unsigned char* pMask = irisMask;
//	for(int j = 0; j < h; j++)
//	{
//		for(int i = 0; i < w; i++)
//		{
//			int dis = sqrt((float)(i - irisRadius) * (i - irisRadius) + (j - irisRadius) * (j - irisRadius));
//			if(f_PNPoly_OPT(xPoints, yPoints, pointNum, i + px, j + py, minX, minY, maxX, maxY) && dis < irisRadius-3)
//				pMask[0] = pMask[1] = pMask[2] = 255;
//			else
//				pMask[0] = pMask[1] = pMask[2] = 0;
//			pMask += 4;
//		}
//	}
//	f_FastMeanFilter(irisMask, w, h, w * 4, 3);
//	pMask = irisMask;
//	for(int j = 0; j < h; j++)
//	{
//		for(int i = 0; i < w; i++)
//		{
//			if(pMask[0] > 128)
//				pMask[0] = pMask[1] = pMask[2] = 255;
//			else
//				pMask[0] = pMask[1] = pMask[2] = 0;
//			pMask += 4;
//		}
//	}
//	f_FastMeanFilter(irisMask, w, h, w * 4, 3);
//	//beauty iris process
//	pMask = irisMask;
//	for(int j = 0; j < h; j++)
//	{
//		for(int i = 0; i < w; i++)
//		{
//			if((pMask[0] + pMask[1] + pMask[2]) / 3 > 0)
//			{
//			    int pos_src = (px + i) * 4 + (py + j) * stride;
//			    int mx = i * mWidth / w;
//			    int my = j * mHeight / h;
//			    int pos_mask = mx * 4 + my * mStride;
//			    int r = srcData[pos_src + 2];
//			    int g = srcData[pos_src + 1];
//			    int b = srcData[pos_src];
//			    int mr = maskData[pos_mask + 2];
//			    int mg = maskData[pos_mask + 1];
//			    int mb = maskData[pos_mask];
//			    float Y, U, V, mY, mU, mV;
//				//YUV
//			    RGB2HSB(r, g, b,&Y, &U, &V);
//			    RGB2HSB(mr, mg, mb, &mY, &mU, &mV);
//			    unsigned char R, G, B;
//			    HSB2RGB(mY, mU, V, &R, &G, &B);
//				//feather mask process
//				int a = (pMask[0] + pMask[1] + pMask[2]) / 3;
//				B = CLIP3((b * (255 - a) + B * a) >> 8, 0, 255);
//				G = CLIP3((g * (255 - a) + G * a) >> 8, 0, 255);
//				R = CLIP3((r * (255 - a) + R * a) >> 8, 0, 255);
//				//YUV color correct
//			 //   a = CLIP3((r + g + b) / 3 * 2, 0, 255);
//				//R = (R * a + r * (255 - a)) / 255;
//				//G = (G * a + g * (255 - a)) / 255;
//				//B = (B * a + b * (255 - a)) / 255;
//				//ratio blending
//				B = CLIP3((b * (100 - ratio) + B * ratio) / 100, 0, 255);
//				G = CLIP3((g * (100 - ratio) + G * ratio) / 100, 0, 255);
//				R = CLIP3((r * (100 - ratio) + R * ratio) / 100, 0, 255);
//
//				srcData[pos_src]     = B;
//				srcData[pos_src + 1] = G;
//				srcData[pos_src + 2] = R;
//			}
//			pMask += 4;
//		}
//	}
//	free(irisMask);
//	return ret;
//};
