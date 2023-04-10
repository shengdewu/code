#include"Commen.h"
#include"f_MakeupBase.h"
#include"f_AffineTransfrom.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
/*************************************************************************
*Function: Makeup base with Nearest Neighbour Interpolation
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*srcKeyPoints: 3 key points of source image.
*mskData: mask image buffer with format bgra32
*mWidth: width of mask image 
*mHeight: height of mask image
*mStride: Stride of mask image
*maskKeyPoints£º 3 key points of blush mask.
*ratio: intensity of effect, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_MakeupBaseN(unsigned char* srcData, int width, int height, int stride, int srcKeyPoints[2 * 3], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 3], int ratio)
{
	int ret = 0; 
	float H[6] = {0};
	f_AffinetransformMetrixCompute((float)maskKeyPoints[0],(float)maskKeyPoints[1],(float)maskKeyPoints[2],(float)maskKeyPoints[3],(float)maskKeyPoints[4],(float)maskKeyPoints[5], (float)srcKeyPoints[0], (float)srcKeyPoints[1], (float)srcKeyPoints[2], (float)srcKeyPoints[3], (float)srcKeyPoints[4], (float)srcKeyPoints[5], H);
	unsigned char* pSrc = srcData;
	int alpha = 128 * ratio / 100;
	int nalpha = 128 - alpha;
	int AR, AG, AB, AA;
	int x0, y0, index_x0y0, index_x0y1, index_x1y0, index_x1y1;
	int disX, disY, tmp1, tmp2;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int x0 = (H[0] * i + H[1] * j + H[2]);
	        int y0 = (H[3] * i + H[4] * j + H[5]);
			if(x0 > 0 && x0 < mWidth - 1 && y0 > 0 && y0 < mHeight - 1)
			{
			    int pos = (x0 << 2) + y0 * mStride;
			    int k = mskData[pos + 3];
			    int b = (k * mskData[pos]     + (255 - k) * pSrc[0]) / 255;
			    int g = (k * mskData[pos + 1] + (255 - k) * pSrc[1]) / 255;
			    int r = (k * mskData[pos + 2] + (255 - k) * pSrc[2]) / 255;
			    pSrc[0] = ((alpha * b + nalpha * pSrc[0]) >> 7);
			    pSrc[1] = ((alpha * g + nalpha * pSrc[1]) >> 7);
			    pSrc[2] = ((alpha * r + nalpha * pSrc[2]) >> 7);
			}
			pSrc += 4;
		}
	}
	return ret;
};
/*************************************************************************
*Function: Makeup base with bilinear interpolation
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*srcKeyPoints: 3 key points of source image.
*mskData: mask image buffer with format bgra32
*mWidth: width of mask image 
*mHeight: height of mask image
*mStride: Stride of mask image
*maskKeyPoints£º 3 key points of blush mask.
*ratio: intensity of effect, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_MakeupBase(unsigned char* srcData, int width, int height, int stride, int srcKeyPoints[2 * 3], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 3], int ratio)
{
	int ret = 0; 
	float H[6] = {0};
	f_AffinetransformMetrixCompute((float)maskKeyPoints[0],(float)maskKeyPoints[1],(float)maskKeyPoints[2],(float)maskKeyPoints[3],(float)maskKeyPoints[4],(float)maskKeyPoints[5], (float)srcKeyPoints[0], (float)srcKeyPoints[1], (float)srcKeyPoints[2], (float)srcKeyPoints[3], (float)srcKeyPoints[4], (float)srcKeyPoints[5], H);
	unsigned char* pSrc = srcData;
	int alpha = 128 * ratio / 100;
	int nalpha = 128 - alpha;
	int AR, AG, AB, AA;
	int x0, y0, index_x0y0, index_x0y1, index_x1y0, index_x1y1;
	int disX, disY, tmp1, tmp2;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			float cx = (H[0] * i + H[1] * j + H[2]);
	        float cy = (H[3] * i + H[4] * j + H[5]);
			if(cx > 0 && cx < mWidth - 1 && cy > 0 && cy < mHeight - 1)
			{

				x0 = (int)CLIP3(floor(cx), 0, mWidth - 2);
				y0 = (int)CLIP3(floor(cy), 0, mHeight - 2);
				index_x0y0 = (x0 << 2) + y0 * mStride;
				index_x1y0 = index_x0y0 + 4;
				index_x0y1 = index_x0y0 + mStride;
				index_x1y1 = index_x0y1 + 4;
				disX = (int)((cx - x0) * 16384);
				disY = (int)((cy - y0) * 16384);	
				tmp1 = mskData[index_x0y0] + ((disX *(mskData[index_x1y0] - mskData[index_x0y0])) >> 14);
				tmp2 = mskData[index_x0y1] + ((disX *(mskData[index_x1y1] - mskData[index_x0y1])) >> 14);
				AB = tmp1 + ((disY * (tmp2 - tmp1)) >> 14);
				tmp1 = mskData[index_x0y0 + 1] + ((disX *(mskData[index_x1y0 + 1] - mskData[index_x0y0 + 1])) >> 14);
				tmp2 = mskData[index_x0y1 + 1] + ((disX *(mskData[index_x1y1 + 1] - mskData[index_x0y1 + 1])) >> 14);
				AG = tmp1 + ((disY * (tmp2 - tmp1)) >> 14);
				tmp1 = mskData[index_x0y0 + 2] + ((disX *(mskData[index_x1y0 + 2] - mskData[index_x0y0 + 2])) >> 14);
				tmp2 = mskData[index_x0y1 + 2] + ((disX *(mskData[index_x1y1 + 2] - mskData[index_x0y1 + 2])) >> 14);
				AR = tmp1 + ((disY * (tmp2 - tmp1)) >> 14);
				tmp1 = mskData[index_x0y0 + 3] + ((disX *(mskData[index_x1y0 + 3] - mskData[index_x0y0 + 3])) >> 14);
				tmp2 = mskData[index_x0y1 + 3] + ((disX *(mskData[index_x1y1 + 3] - mskData[index_x0y1 + 3])) >> 14);
				AA = tmp1 + ((disY * (tmp2 - tmp1)) >> 14);

				int pos = (x0 << 2) + y0 * mStride;
			    int k = AA;
			    int b = (k * AB     + (255 - k) * pSrc[0]) / 255;
			    int g = (k * AG + (255 - k) * pSrc[1]) / 255;
			    int r = (k * AR + (255 - k) * pSrc[2]) / 255;
			    pSrc[0] = ((alpha * b + nalpha * pSrc[0]) >> 7);
			    pSrc[1] = ((alpha * g + nalpha * pSrc[1]) >> 7);
			    pSrc[2] = ((alpha * r + nalpha * pSrc[2]) >> 7);
			}
			pSrc += 4;
		}
	}
	return ret;
};