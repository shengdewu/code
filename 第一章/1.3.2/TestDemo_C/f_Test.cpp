#include"f_Test.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"TRGB2HSV.h"
#include"Commen.h"
int f_HSVAdjust(unsigned char *srcData, int width, int height, int stride, float hIntensity, float sIntensity, float vIntensity)
{
	int ret = 0;
	if(srcData == NULL)
	{
		printf("input image is null!");
		return -1;
	}
	//Process
    unsigned char R, G, B;
	float h = 0, s = 0, v = 0;
	unsigned char* pSrc = srcData;
	int offset = stride - width * 4;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			B = pSrc[0];
			G = pSrc[1];
			R = pSrc[2];
			RGB2HSV(R, G, B, &h, &s, &v);
			h = h + hIntensity > 360 ? h + hIntensity - 360 : h + hIntensity;
			s = CLIP3(s + sIntensity, 0, 1.0f);
			v = CLIP3(v + vIntensity, 0, 1.0f);
			HSV2RGB(h, s, v, &R, &G, &B);
			pSrc[0] = B;
			pSrc[1] = G;
			pSrc[2] = R;
			pSrc += 4;
		}
		pSrc += offset;
	}
	return ret;
};