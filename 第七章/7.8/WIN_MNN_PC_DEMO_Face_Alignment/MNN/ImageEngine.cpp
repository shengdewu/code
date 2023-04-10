#include"ImageEngine.h"
#include"MNN.h"
#include"f_SF_FaceDetect.h"
int SF_MNN_Initialize(char* fdmodelPath, char* famodelPath, int tensorSize, int stdMode, int numThread)
{
	int ret = 0;
	ret = f_MNN_Initialize(fdmodelPath, famodelPath, tensorSize, stdMode, numThread);
	return ret;
};
void SF_MNN_Unitialize()
{
	return f_MNN_Unitialize();
};
/************************************************************
*Function:  SF_FaceskinSegment
*Description: FaceSkin Segment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*isThreshold: 1-binary process, other's not.
*Return :0-OK,or failed
************************************************************/
int SF_FaceskinSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold)
{
	int ret = f_MNN_Process(srcData, width, height, stride);
	if (isThreshold)
	{
		unsigned char* pSrc = srcData;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				int gray = (pSrc[0] + pSrc[1] + pSrc[2]) / 3 < 128 ? 0 : 255;
				pSrc[0] = gray;
				pSrc[1] = gray;
				pSrc[2] = gray;
				pSrc += 4;
			}

		}
	}
	return ret;
};
/************************************************************
*Function:  SF_FaceDetect
*Description: Face detection
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*faceRects: face info array, [faceNum, x0, y0, w0, h0, x1, y1, w1, h1, x2, y2, w2, h2, x3, y3, w3, h3].
*Return :0-OK,or failed
************************************************************/
int SF_FaceDetect(unsigned char* srcData, int width, int height, int stride, int faceRects[1 + 4 * 4])
{
	int ret = 0;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(maskData, srcData, sizeof(unsigned char) * height * stride);
	ret = SF_FaceskinSegment(maskData, width, height, stride, 1);
	ret = f_FaceDetect(maskData, width, height, stride, faceRects);
	free(maskData);
	return ret;
};

/************************************************************
*Function:  SF_FaceAlignment
*Description: Face alignment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*faceRect: face rect info.
*facePoints:face points to output
*Return :0-OK,or failed
************************************************************/
int SF_FaceAlignment(unsigned char* srcData, int width, int height, int stride, int faceRect[], float facePoints[])
{
	int ret = 0;
	int faceSize = 96;
	unsigned char* faceImg = (unsigned char*)malloc(sizeof(unsigned char) * faceSize * faceSize * 4);
	int h = faceRect[3];
	int w = faceRect[2];
	unsigned char* pFace = faceImg;
	for (int j = 0; j < faceSize; j++)
	{
		for (int i = 0; i < faceSize; i++)
		{
			int cx = CLIP3(i * w / faceSize + faceRect[0], 0, width - 1);
			int cy = CLIP3(j * h / faceSize + faceRect[1], 0, height - 1);
			int pos = cx * 4 + cy * stride;
			pFace[0] = srcData[pos];
			pFace[1] = srcData[pos + 1];
			pFace[2] = srcData[pos + 2];
			pFace[3] = 255;
			pFace += 4;
		}
	}
	ret = f_MNN_FA_Process(faceImg, faceSize, faceSize, faceSize * 4, facePoints);
	for (int i = 0; i < 101; i++)
	{
		float cx = faceRect[0] + facePoints[2 * i] * w;
		float cy = faceRect[1] + facePoints[2 * i + 1] * h;
		facePoints[2 * i] = cx / width;
		facePoints[2 * i + 1] = cy / height;
	}
	free(faceImg);
	return ret;
};
