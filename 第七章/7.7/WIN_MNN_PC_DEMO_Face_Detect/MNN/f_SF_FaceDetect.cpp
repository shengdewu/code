#include"f_SF_FaceDetect.h"
#include"Commen.h"
#include"f_SF_ImgBase_ConnectComponentLabel.h"

float RectIOU(int left, int top, int right, int bottom, int left1, int top1, int right1, int bottom1)
{
    float ratio = 0;
	int x = MIN2(left, left1);
	int y = MIN2(top, top1);
	int px = MAX2(right, right1);
	int py = MAX2(bottom, bottom1);
	int s = (right - left) * (bottom - top);
	int smax = (px - x) * (py - y);
	ratio = (float)smax / s;
    return ratio; 
}
/************************************************************
*Function:  f_FaceDetect
*Description: Face detection
*Params:
*maskData:  binary image data of source image, one channel
*mWidth  : mask image width
*mHeight : mask image height
*mStride: mask image stride
*FaceRectInfos: Face Rectangle array, [faceNum, x0, y0, w0, h0, x1, y1, w1, h1, x2, y2, w2, h2, x3, y3, w3, h3], faceNum max = 4.
*Return :0-OK,or failed
************************************************************/
int f_FaceDetect(unsigned char* maskData, int mWidth, int mHeight, int mStride, int FaceRectInfos[1 + 4 * 4])
{
	int ret = 0;
	int* binaryData = (int*)malloc(sizeof(int) * mWidth * mHeight);
	int* binaryNVData = (int*)malloc(sizeof(int) * mWidth * mHeight);
	unsigned char* pMask = maskData;
	int* pBinary = binaryData;
	int* pNVBinary = binaryNVData;
	for(int j = 0; j < mHeight; j++)
	{
		for(int i = 0; i < mWidth; i++)
		{
			if(pMask[0] + pMask[1] + pMask[2] < 250)
				pMask[0] = pMask[1] = pMask[2] = 0;
			else
				pMask[0] = pMask[1] = pMask[2] = 255;
			*pBinary = pMask[0];
			if(i == 0 || i == mWidth - 1 || j == 0 || j == mHeight - 1)
			{
				*pBinary = 0;
			}
			*pNVBinary = 255 - pMask[0];
			pMask += 4;
			pBinary++;
			pNVBinary++;
		}
	}
	int labelCount = 0, labelNVCount = 0;
	ret = ConnectComponentLabel(binaryData, mWidth, mHeight, &labelCount);
	TRect *labelRects = (TRect*)malloc(sizeof(TRect) * (labelCount + 1));
	long* labelAreaCount = (long*) malloc(sizeof(long) * (labelCount + 1));
	memset(labelRects, 0, sizeof(TRect) * (labelCount + 1));
	memset(labelAreaCount, 0, sizeof(long) * (labelCount + 1));
	ret = ConnectComponentCal(binaryData, mWidth, mHeight,labelAreaCount, labelRects);

	ret = ConnectComponentLabel(binaryNVData, mWidth, mHeight, &labelNVCount);
	TRect *labelNVRects = (TRect*)malloc(sizeof(TRect) * (labelNVCount + 1));
	long* labelNVAreaCount = (long*) malloc(sizeof(long) * (labelNVCount + 1));
	memset(labelNVRects, 0, sizeof(TRect) * (labelNVCount + 1));
	memset(labelNVAreaCount, 0, sizeof(long) * (labelNVCount + 1));
	ret = ConnectComponentCal(binaryNVData, mWidth, mHeight,labelNVAreaCount, labelNVRects);
	int*FaceRectInfo = (int*)malloc(sizeof(int) * ((labelCount + 1) * 4 + 1));
	int faceCount = 0;
	float iou = 0;
	for(int i = 1; i < labelCount + 1; i++)
	{
		int x = (labelRects + i)->Pos.X;
		int y = (labelRects + i)->Pos.Y;
		int w = (labelRects + i)->Right - (labelRects + i)->Left;
		int h = (labelRects + i)->Bottom - (labelRects + i)->Top;
		if(w <= 0 && h <= 0)
			continue;
		////////////////////////////////////////////////////////
		int innerCount = 0;
		for(int m = 1; m < labelNVCount; m++)
		{
			if(((labelNVRects + m)->Right - (labelNVRects + m)->Left) * ((labelNVRects + m)->Bottom - (labelNVRects + m)->Top) < 15)
				continue;
			iou = RectIOU((labelRects + i)->Left,(labelRects + i)->Top,(labelRects + i)->Right,(labelRects + i)->Bottom,(labelNVRects + m)->Left,(labelNVRects + m)->Top,(labelNVRects + m)->Right,(labelNVRects + m)->Bottom);
			if(iou < 1.3f && iou > 0.95f)
			{
				innerCount++;
			}
		}
	    if(innerCount >= 2)
		{
			FaceRectInfo[1 + faceCount * 4] = x;
		    FaceRectInfo[1 + faceCount * 4 + 1] = y;
		    FaceRectInfo[1 + faceCount * 4 + 2] = (labelRects + i)->Right;
		    FaceRectInfo[1 + faceCount * 4 + 3] = (labelRects + i)->Bottom;
			faceCount++;
		}
	}
	FaceRectInfo[0] = faceCount;
	for(int i = 0; i < faceCount; i++)
	{
		if(i < 4)
		{
			FaceRectInfos[1 + i * 4] = FaceRectInfo[1 + i * 4];
			FaceRectInfos[1 + i * 4 + 1] = FaceRectInfo[1 + i * 4 + 1];
			FaceRectInfos[1 + i * 4 + 2] = FaceRectInfo[1 + i * 4 + 2];
			FaceRectInfos[1 + i * 4 + 3] = FaceRectInfo[1 + i * 4 + 3];
		}
	}
	FaceRectInfos[0] = MIN2(FaceRectInfo[0], 4);
	for(int i = 0; i < FaceRectInfos[0]; i++)
	{
		int x = (FaceRectInfo[1 + i * 4] + FaceRectInfo[1 + i * 4 + 2]) / 2;
		int y = (FaceRectInfo[1 + i * 4 + 1] + FaceRectInfo[1 + i * 4 + 3]) / 2;
		//int max = MAX2(FaceRectInfo[1 + i * 4 + 2] - FaceRectInfo[1 + i * 4], FaceRectInfo[1 + i * 4 + 3] - FaceRectInfo[1 + i * 4 + 1]);
		//max = (max >> 1) + 5;// +max / 15;
		int max = MAX2(FaceRectInfo[1 + i * 4 + 2] - FaceRectInfo[1 + i * 4], FaceRectInfo[1 + i * 4 + 3] - FaceRectInfo[1 + i * 4 + 1]);
		max = max * 4 / 6;
		FaceRectInfos[1 + i * 4 + 0] = x - max;
		FaceRectInfos[1 + i * 4 + 1] = y - max;
		FaceRectInfos[1 + i * 4 + 2] = max * 2;
		FaceRectInfos[1 + i * 4 + 3] = max * 2;
	}
	free(FaceRectInfo);
	free(binaryData);
	free(labelRects);
	free(labelAreaCount);
	free(binaryNVData);
	free(labelNVRects);
	free(labelNVAreaCount);
	return ret;
};


