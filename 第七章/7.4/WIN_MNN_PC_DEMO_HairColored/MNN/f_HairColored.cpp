#include"f_HairColored.h"
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"TRGB2HSV.h"
#include"Commen.h"
#include"f_LUTFilter.h"
#include"f_LaplaceSharpen.h"


inline int f_TFastGaussFilterOneChannel(unsigned char* srcData, int width, int height, unsigned char* dstData, float r)
{
	int ret = 0;
	int radius = (int)(3.0f * r);
	if (radius == 0)
		return 0;
	if (radius > MIN2(width / 2, height / 2))
		radius = MIN2(width, height) / 3;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * width);
	unsigned char *kernel = (unsigned char *)malloc(2 * radius + 1);
	int* map = (int*)malloc(sizeof(int) * height);
	if (NULL == tempData || NULL == kernel || NULL == map)
		return 1;
	int totalWei = 0;
	int i, j, k;
	float sigma = (float)(3.0f * r / 3.0f);
	float stdsigma = 2.0f * sigma * sigma;
	for (i = -radius; i <= radius; i++)
	{
		kernel[i + radius] = (unsigned char)(exp(-(float)i * i / stdsigma) * 128);
		totalWei += kernel[i + radius];
	}
	for (i = 0; i < height; i++)
	{
		map[i] = i * width;
	}
	int temp = 0;
	int v = 0;
	int K = 0;
	int rem = 0;
	int t = 0;
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			temp = 0;
			for (k = -radius; k <= radius; k++)
			{
				rem = i + k < 0 ? -(i + k) : (i + k > width - 1 ? (width + width - 2 - i - k) : (i + k));
				//rem = (abs(i + k) % width);
				t = rem + map[j];
				K = kernel[k + radius];
				temp += srcData[t] * K;
			}
			v = i + map[j];
			tempData[v] = temp / totalWei;
		}
	}
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			temp = 0;
			for (k = -radius; k <= radius; k++)
			{
				rem = j + k < 0 ? -(j + k) : (j + k > height - 1 ? (height + height - 2 - j - k) : (j + k));
				//rem = (abs(j + k) % height);
				t = map[rem] + i;
				K = kernel[k + radius];
				temp += tempData[t] * K;
			}
			v = i + map[j];
			dstData[v] = temp / totalWei;
		}
	}
	free(tempData);
	free(kernel);
	free(map);
	return ret;
};
inline int f_TFastGaussFilter(unsigned char* srcData, int width, int height, int stride, unsigned char* dstData, float r)
{
	int ret = 0;
	int radius = (int)(3 * r);
	if (radius == 0)
		return 0;
	if (srcData == NULL)
		return 1;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	unsigned char *kernel = (unsigned char *)malloc(2 * radius + 1);
	int* map = (int*)malloc(sizeof(int) * height);
	if (NULL == tempData || NULL == kernel || NULL == map)
		return 1;
	int totalWei = 0;
	int i, j, k;
	float sigma = 3 * r / 3.0f;
	float stdsigma = 2.0f * sigma * sigma;
	for (i = -radius; i <= radius; i++)
	{
		kernel[i + radius] = (unsigned char)(exp(-(float)i*i / stdsigma) * 128);
		totalWei += kernel[i + radius];
	}
	for (i = 0; i < height; i++)
	{
		map[i] = i * stride;
	}
	int tempR = 0, tempG = 0, tempB = 0, tempA = 0;
	int v = 0;
	int K = 0;
	int rem = 0;
	int t = 0;
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			tempR = 0; tempG = 0; tempB = 0; tempA = 0;
			for (k = -radius; k <= radius; k++)
			{
				rem = i + k < 0 ? -(i + k) : (i + k > width - 1 ? (width + width - 2 - i - k) : (i + k));
				//rem = (abs(i + k) % width);
				t = (rem << 2) + map[j];
				K = kernel[k + radius];
				if (srcData[t + 3] == 0)
				{
					tempB += 255 * K;
					tempG += 255 * K;
					tempR += 255 * K;
				}
				else
				{
					tempB += srcData[t] * K;
					tempG += srcData[t + 1] * K;
					tempR += srcData[t + 2] * K;
				}
				tempA += srcData[t + 3] * K;
			}
			v = (i << 2) + map[j];
			tempData[v] = tempB / totalWei;
			tempData[v + 1] = tempG / totalWei;
			tempData[v + 2] = tempR / totalWei;
			tempData[v + 3] = tempA / totalWei;
		}
	}
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			tempR = 0; tempG = 0; tempB = 0; tempA = 0;
			for (k = -radius; k <= radius; k++)
			{
				rem = j + k < 0 ? -(j + k) : (j + k > height - 1 ? (height + height - 2 - j - k) : (j + k));
				//rem = (abs(j + k) % height);
				t = map[rem] + (i << 2);
				K = kernel[k + radius];
				tempB += tempData[t] * K;
				tempG += tempData[t + 1] * K;
				tempR += tempData[t + 2] * K;
				tempA += tempData[t + 3] * K;
			}
			v = (i << 2) + map[j];
			dstData[v] = tempB / totalWei;
			dstData[v + 1] = tempG / totalWei;
			dstData[v + 2] = tempR / totalWei;
			dstData[v + 3] = tempA / totalWei;
		}
	}
	free(tempData);
	free(kernel);
	free(map);
	return ret;
};
int f_HairColored(unsigned char* srcData, int width, int height, int stride, unsigned char* hairMask, unsigned char* filtermap, unsigned char* colorMask, int ratio)
{
	if (ratio == 0)
		return 0;
	unsigned char* mask = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* pSrc = srcData;
	unsigned char* pTemp = hairMask;
	pSrc = mask;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			*pSrc++ = (255 - (pTemp[0] + pTemp[1] + pTemp[2]) / 3 < 128 ? 0 : 255);
			pTemp += 4;
		}
	}
	unsigned char* gaussmask = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	float radius = MAX2(2.0f * width * height / (1000 * 1000.0f), 1.0f);
	f_TFastGaussFilterOneChannel(mask, width, height, gaussmask, radius);
	unsigned char* pMsk = gaussmask;
	int left = 10000, right = 0, top = 10000, bottom = 0, w, h;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (*pMsk == 0)/////////////////the hair area is black
			{
				top = MIN2(top, j);
				bottom = MAX2(bottom, j);
				left = MIN2(left, i);
				right = MAX2(right, i);
			}
			if (*pMsk > 0)
			{
				*pMsk = 255;
			}
			pMsk++;
		}
	}
	w = right - left;
	h = bottom - top;
	f_TFastGaussFilterOneChannel(gaussmask, width, height, mask, radius * 2.0f);
	int k = ratio * 128 / 100;
	int nk = 128 - k;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	memcpy(tempData, srcData, sizeof(unsigned char) * stride * height);
	f_LUTFilter(tempData, width, height, stride, filtermap);
	f_LaplaceSharpen(tempData, width, height, stride, 0);
	int pos = 0, x = 0, y = 0;
	unsigned char r = 0, g = 0, b = 0;
	float colorH = 0, colorS = 0, colorV = 0;
	float srcH = 0, srcS = 0, srcV = 0;
	int* xtable = (int*)malloc(sizeof(int) * w);
	int* ytable = (int*)malloc(sizeof(int) * h);
	for (int i = 0; i < w; i++)
	{
		xtable[i] = i * 200 / w;
	}
	for (int i = 0; i < h; i++)
	{
		ytable[i] = i * 200 / h;
	}
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int K = (mask[i + left + (j + top) * width] << 7) / 255;
			if (128 - K > 0)
			{
				x = xtable[i];
				y = ytable[j];
				pos = (x << 2) + y * 800;
				RGB2HSV(colorMask[pos + 2], colorMask[pos + 1], colorMask[pos + 0], &colorH, &colorS, &colorV);
				x = i + left;
				y = j + top;
				pos = (x << 2) + y * stride;
				RGB2HSV(tempData[pos + 2], tempData[pos + 1], tempData[pos + 2], &srcH, &srcS, &srcV);
				HSV2RGB(colorH, colorS, srcV, &r, &g, &b);

				b = (b * (128 - K) + srcData[pos + 0] * K) >> 7;
				g = (g * (128 - K) + srcData[pos + 1] * K) >> 7;
				r = (r * (128 - K) + srcData[pos + 2] * K) >> 7;
				srcData[pos + 0] = (b * k + srcData[pos + 0] * nk) >> 7;
				srcData[pos + 1] = (g * k + srcData[pos + 1] * nk) >> 7;
				srcData[pos + 2] = (r * k + srcData[pos + 2] * nk) >> 7;
			}
		}
	}
	free(xtable);
	free(ytable);
	free(mask);
	free(gaussmask);
	free(tempData);
	return 0;
};