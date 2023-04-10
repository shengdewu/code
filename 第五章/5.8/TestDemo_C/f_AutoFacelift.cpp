#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_IDW.h"
#include"f_AutoFacelift.h"

/*************************************************************************
*Function: AutoFaceLift
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*facePoints: 101 face points of image.
*intensity: intensity of facelift, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_AutoFacelift(unsigned char* srcData, int width, int height, int stride, int facePoints[], int intensity)
{
	int ret = 0;
	float K1, K2;
	float P5x = facePoints[2 * 1];
	float P5y = facePoints[2 * 1 + 1];
	float P6x = facePoints[2 * 5];
	float P6y = facePoints[2 * 5 + 1];
	float P7x = facePoints[2 * 9];
	float P7y = facePoints[2 * 9 + 1];
	float P8x = facePoints[2 * 13];
	float P8y = facePoints[2 * 13 + 1];
	float P9x = facePoints[2 * 17];
	float P9y = facePoints[2 * 17 + 1];
	float P0x = facePoints[2 * 98];
	float P0y = facePoints[2 * 98 + 1];
	//compute P' after facelift
	float K = 0.1f * intensity / 100;
	float P6X, P6Y, P8X, P8Y;
	P6X = P6x + (P0x - P6x) * K;
	P6Y = P6y + (P0y - P6y) * K;
	P8X = P8x + (P0x - P8x) * K;
	P8Y = P8y + (P0y - P8y) * K;
	//compute face rectangle
	float dis = sqrt(((float)facePoints[2 * 98] - facePoints[2 * 9])*((float)facePoints[2 * 98] - facePoints[2 * 9]) + ((float)facePoints[2 * 98 + 1] - facePoints[2 * 9 + 1])*((float)facePoints[2 * 98 + 1] - facePoints[2 * 9 + 1])) * 1.1;
	int minx = CLIP3(P0x - dis, 0, width - 1);
	int miny = CLIP3(P0y - dis, 0, height - 1);
	int maxx = CLIP3(P0x + dis, 0, width - 1);
	int maxy = CLIP3(P0y + dis, 0, height - 1);
	int aPoints[2 * 9] = {minx, miny, minx, maxy, maxx, maxy, maxx, miny, P5x, P5y, P6x, P6y, P7x, P7y, P8x, P8y, P9x, P9y};
	int bPoints[2 * 9] = {minx, miny, minx, maxy, maxx, maxy, maxx, miny, P5x, P5y, P6X, P6Y, P7x, P7y, P8X, P8Y, P9x, P9y};
	ret = f_IDW(srcData, width, height, stride, bPoints, aPoints, 9);
	return ret;
};