#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_BigEye.h"
#include"f_AutoBigeye.h"

/*************************************************************************
*Function: Auto Bigeye
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*facePoints: 101 face points of image.
*Return: 0-OK,other failed
**************************************************************************/
int f_AutoBigeye(unsigned char* srcData, int width, int height, int stride, int facePoints[])
{
	int ret = 0;
	//cenx, ceny
	int cenX_left = facePoints[2 * 95];
	int cenY_left = facePoints[2 * 95 + 1];
	int cenX_right = facePoints[2 * 96];
	int cenY_right = facePoints[2 * 96 + 1];
	//compute radius
	float dis_left = sqrt((float)(facePoints[2 * 39] - facePoints[2 * 45]) * (facePoints[2 * 39] - facePoints[2 * 45]) + (facePoints[2 * 39 + 1] - facePoints[2 * 45 + 1]) * (facePoints[2 * 39 + 1] - facePoints[2 * 45 + 1]));
	float dis_right = sqrt((float)(facePoints[2 * 51] - facePoints[2 * 57]) * (facePoints[2 * 51] - facePoints[2 * 57]) + (facePoints[2 * 51 + 1] - facePoints[2 * 57 + 1]) * (facePoints[2 * 51 + 1] - facePoints[2 * 57 + 1]));
	int radius = MAX2(dis_left, dis_right);
	//compute intensity
	int intensity = 15 * 512 * 512 / (width * height);
	ret = f_BigEye(srcData, width, height, stride, cenX_left, cenY_left, radius, intensity);
	ret = f_BigEye(srcData, width, height, stride, cenX_right, cenY_right, radius, intensity);
	return ret;
};