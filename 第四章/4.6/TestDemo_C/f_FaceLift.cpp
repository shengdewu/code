#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_FaceLift.h"

/*************************************************************************
*Function: FaceLift
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*centerX: center x of warp circle area 
*centerY: center y of warp circle area
*rmax: radius of warp circle area
*mx: x of the point after moving
*my: y of the point after moving
*strength: warp intensity, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_FaceLift(unsigned char* srcData, int width, int height, int stride, int centerX, int centerY, int rmax, int mx, int my, int strength)
{
	int ret = 0;
	unsigned char* pSrc = srcData;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			float dis = sqrt((float)(centerX - i) * (centerX - i) + (centerY - j) * (centerY - j));
			if(dis < rmax)
			{
				float xc = (i - centerX) * (i - centerX) + (j - centerY) * (j - centerY);
				float mc = (mx - centerX) * (mx - centerX) + (my - centerY) * (my - centerY);
				float tx = (rmax * rmax - xc);
				float d = tx / (tx + mc *  100.0f / strength);
				d = d * d;
				int px = CLIP3(i - (mx - centerX) * d * (1.0f - dis / rmax), 0, width - 1);
				int py = CLIP3(j - (my - centerY) * d * (1.0f - dis / rmax), 0, height - 1);
				int pos = (px << 2) + py * stride;
				pSrc[0] = tempData[pos];
				pSrc[1] = tempData[pos + 1];
				pSrc[2] = tempData[pos + 2];
			}
			pSrc += 4;
		}
	}
	free(tempData);
	return ret;
};