#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_PerspectiveTransform.h"

/*************************************************************************
*Function: Perspective Transform
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*H: perspective matrix,[a11, a12, a13, a21, a22, a23, a31, a32, a33]
*Return: 0-OK,other failed
**************************************************************************/
int f_PerspectiveTransform(unsigned char* srcData, int width, int height, int stride, float H[6])
{
	int ret = 0;
	int tx, ty, pos;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memset(tempData, 0, sizeof(unsigned char) * height * stride);
	unsigned char* pSrc = tempData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			float w = H[6] * i / width + H[7] * j / height + 1.0f;
			tx = (H[0] * i / width + H[1] * j / height + H[2] / width ) / w * width;
			ty = (H[3] * i / width + H[4] * j / height + H[5] / height) / w * height;
			tx = CLIP3(tx, 0, width - 1);
			ty = CLIP3(ty, 0, height - 1);
			if(tx == width - 1 || ty == height - 1 || tx == 0 || ty == 0)
			{
				pSrc[0] = 0;
				pSrc[1] = 0;
				pSrc[2] = 0;
			}
			else
			{
		        pos = (tx << 2) + ty * stride;
			    pSrc[0] = srcData[pos]    ;
			    pSrc[1] = srcData[pos + 1];
			    pSrc[2] = srcData[pos + 2];
			}
			pSrc[3] = 255;
			pSrc += 4;
		}
	}
	memcpy(srcData, tempData, sizeof(unsigned char) * height * stride);
	free(tempData);
	return ret;
};