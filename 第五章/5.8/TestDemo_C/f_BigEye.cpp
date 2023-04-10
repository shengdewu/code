#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_BigEye.h"


/*************************************************************************
*Function: BigEye
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*cenX: X of center points for eye
*cenY: Y of center points for eye
*radius: Radius of warp area
*intensity: Intensity of eye big processing
*Return: 0-OK,other failed
**************************************************************************/
int f_BigEye(unsigned char* srcData, int width, int height, int stride, int cenX, int cenY, int radius, int intensity)
{
	int ret = 0;
	int nx = CLIP3(cenX - radius, 0, width - 1);
	int ny = CLIP3(cenY - radius, 0, height - 1);
	int nw = CLIP3(cenX + radius, 0, width - 1);
	int nh = CLIP3(cenY + radius, 0, height - 1);
	int D = radius * radius;
	float k0 = intensity / 100.0f;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	for(int j = ny; j < nh; j++)
	{
		for(int i = nx; i < nw; i++)
		{
			float dis = (i - cenX) * (i - cenX) + (j - cenY) * (j - cenY);
			if(dis < D)
			{
				float k = 1.0f - k0 * (1.0f - dis / D);

				//////////////////可以使用双线性插值，效果更好///////////////////////////////////////
				int px = CLIP3((i - cenX) * k + cenX, 0, width - 1);
				int py = CLIP3((j - cenY) * k + cenY, 0, height - 1);
				/////////////////////////////////////////////////////////////////////////////////////
				int pos_new = px * 4 + py * stride;
				int pos = i * 4 + j * stride;
				srcData[pos] = tempData[pos_new];
				srcData[pos + 1] = tempData[pos_new + 1];
				srcData[pos + 2] = tempData[pos_new + 2];
			}
		}
	}
	free(tempData);
	return ret;
};