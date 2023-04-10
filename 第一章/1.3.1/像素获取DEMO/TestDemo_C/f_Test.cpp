#include"f_Test.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>

int f_getPixel(unsigned char *srcData, int width, int height, int stride, int x, int y, int rgba[3])
{
	x = x < 0 ? 0 : (x > width - 1 ? width - 1 : x);
	y = y < 0 ? 0 : (y > height - 1 ? height - 1 : y);
	int ret = 0;
	if(srcData == NULL)
	{
		printf("input image is null!");
		return -1;
	}
	//Process
	int pos = x * 4 + y * stride;
	rgba[0] = srcData[pos + 2];
    rgba[1] = srcData[pos + 1];
	rgba[2] = srcData[pos + 0];
	rgba[3] = srcData[pos + 3];
	return ret;
};