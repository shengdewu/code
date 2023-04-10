#include"f_SphereFilter.h"
#include"Commen.h"
#include<stdlib.h>
#include<string.h>
#include<math.h>
int f_ConcaveMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k)
{
	x = CLIP3(x, 0, width - 1);
	y = CLIP3(y, 0, height - 1);
	k = MAX2(k, 0);
	int radius = 0;
    float theta = 0;
    int tX = 0;
    int tY = 0;
    int mapX = 0;
    int mapY = 0;
    int mapR = 0;
	unsigned char* pSrc = srcData;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	int offset = stride - width * 4;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			tX = i - x;
            tY = j - y;
			radius = k;;
			float distance = (i - x) * (i - x) + (j - y) * (j - y);
			float dis = sqrt(distance);
			if(distance <= k * k && distance > 0)
			{
				mapX = floor(dis * (i - x) / k + x);
				mapY = floor(dis * (j - y) / k + y);
				pSrc[0] = tempData[mapX * 4 + mapY * stride];
			    pSrc[1] = tempData[mapX * 4 + mapY * stride + 1];
			    pSrc[2] = tempData[mapX * 4 + mapY * stride + 2];
			}
			pSrc += 4;
		}
		pSrc += offset;
	}
	free(tempData);
	return 0;
}
int f_ConvexMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k)
{
	x = CLIP3(x, 0, width - 1);
	y = CLIP3(y, 0, height - 1);
	k = MAX2(k, 0);
	int radius = 0;
	float theta = 0;
	int tX = 0;
	int tY = 0;
	int mapX = 0;
	int mapY = 0;
	int mapR = 0;
	unsigned char* pSrc = srcData;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	int offset = stride - width * 4;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			tX = i - x;
			tY = j - y;
			theta = atan2((float)tY, (float)tX);
			radius = (int)sqrt((float)(tX * tX + tY * tY));
			mapR = (int)(sqrt((float)radius * k));
			mapX = CLIP3(x + (int)(mapR * cos(theta)), 0, width - 1);
			mapY = CLIP3(y + (int)(mapR * sin(theta)), 0, height - 1);
			pSrc[0] = tempData[mapX * 4 + mapY * stride];
			pSrc[1] = tempData[mapX * 4 + mapY * stride + 1];
			pSrc[2] = tempData[mapX * 4 + mapY * stride + 2];
			pSrc += 4;
		}
		pSrc += offset;
	}
	free(tempData);
	return 0;
}

