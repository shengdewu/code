#include"f_Saturation.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"

int f_Saturation(unsigned char *srcData, int width, int height, int stride, int saturation)
{
	int ret = 0;
	if(saturation == 0)
		return ret;
    unsigned char* pSrc = srcData;
	int r, g, b, rgbMin, rgbMax;
	saturation = CLIP3(saturation,-100,100);
	int k = saturation / 100.0f * 128;
	int alpha = 0;
	int offset = stride - width * 4;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			r = pSrc[2];
			g = pSrc[1];
			b = pSrc[0];
			rgbMin = MIN2(MIN2(r, g), b);
			rgbMax = MAX2(MAX2(r, g), b);
			int delta = (rgbMax - rgbMin);
			int value = (rgbMax + rgbMin);
			if(delta ==0)
			{
				pSrc += 4;
				continue;
			}			
			int L = value >> 1;
			int S = L < 128 ? (delta << 7) / value : (delta << 7) / (510 - value);
			if(k >= 0)
			{
				alpha = k + S >= 128 ? S : 128 - k;
				alpha = 128 * 128 / alpha - 128;
			}
			else
				alpha = k;
			r = r + ((r - L) * alpha >> 7);
			g = g + ((g - L) * alpha >> 7);
			b = b + ((b - L) * alpha >> 7);
			pSrc[0] = CLIP3(b, 0, 255);
			pSrc[1] = CLIP3(g, 0, 255);
			pSrc[2] = CLIP3(r, 0, 255);
			pSrc += 4;
		}
		pSrc += offset;
	}
	return ret;
};

