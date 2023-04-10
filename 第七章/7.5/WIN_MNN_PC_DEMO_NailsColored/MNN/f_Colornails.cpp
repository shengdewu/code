#include"f_Colornails.h"
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"TRGB2HSV.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

inline void RGBToHCV(int r, int g, int b, float*h, float*c, float *v)
{
	float R = r / 255.0f;
	float G = g / 255.0f;
	float B = b / 255.0f;
	float p0 = 0, p1 = 0, p2 = 0, p3 = 0, q0 = 0, q1 = 0, q2 = 0, q3 = 0;
	if (G < B)
	{
		p0 = B;
		p1 = G;
		p2 = -1.0f;
		p3 = 2.0 / 3.0f;
	}
	else
	{
		p0 = G;
		p1 = B;
		p2 = 0;
		p3 = -1.0 / 3.0f;
	}
	if (R < p0)
	{
		q0 = p0;
		q1 = p1;
		q2 = p3;
		q3 = R;
	}
	else
	{
		q0 = R;
		q1 = p1;
		q2 = p2;
		q3 = p0;
	}
	float C = q0 - MIN2(q3, q1);
	float H = fabs((q3 - q1) / (6.0f * C + 1e-7) + q2);
	float V = q0;
	*h = H;
	*c = C;
	*v = V;
}
inline void RGBToHSLMK(int r, int g, int b, float* h, float*s, float*l)
{
	float hcv_h = 0, hcv_c = 0, hcv_v = 0;
	RGBToHCV(r, g, b, &hcv_h, &hcv_c, &hcv_v);
	float lum = hcv_v - hcv_c * 0.5f;
	float sat = hcv_c / (1.0f - fabs(2.0f * lum - 1.0f) + 1e-7);
	*h = hcv_h;
	*s = sat;
	*l = lum;
}
inline void HUEToRGB(float hue, float * r, float * g, float *b)
{
	*r = CLIP3(fabs(6.0f * hue - 3.0f) - 1.0f, 0, 1.0f);
	*g = CLIP3(2.0f - fabs(6.0f * hue - 2.0f), 0, 1.0f);
	*b = CLIP3(2.0f - fabs(6.0f * hue - 4.0f), 0, 1.0f);
}
inline void HSLToRGBMK(float h, float s, float l, int *r, int *g, int *b)
{
	float R = 0, G = 0, B = 0;
	HUEToRGB(h, &R, &G, &B);
	float c = (1.0f - fabs(2.0f * l - 1.0f)) * s;
	R = (R - 0.5f) * c + l;
	G = (G - 0.5f) * c + l;
	B = (B - 0.5f) * c + l;
	*r = CLIP3(R * 255.0f, 0, 255);
	*g = CLIP3(G * 255.0f, 0, 255);
	*b = CLIP3(B * 255.0f, 0, 255);
}
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
int f_Colornails(unsigned char* srcData, int width, int height, int stride, unsigned char* maskData, int mWidth, int mHeight, int mStride, int R, int G, int B, int ratio)
{
	int ret = 0;
	//color enhancement  lut 256
	unsigned char Map[256] = { 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 9, 9, 10, 10, 11, 11, 11, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17, 18, 18, 19, 19, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 39, 41, 41, 42, 43, 43, 45, 45, 46, 47, 47, 49, 49, 50, 51, 52, 53, 54, 54, 55, 56, 57, 58, 59, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 68, 70, 71, 71, 72, 74, 75, 75, 77, 78, 78, 80, 80, 81, 83, 83, 85, 85, 86, 87, 88, 89, 90, 91, 93, 93, 95, 95, 97, 98, 98, 100, 100, 101, 103, 104, 104, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 132, 133, 133, 134, 135, 136, 137, 139, 140, 141, 142, 144, 145, 146, 148, 148, 149, 150, 152, 153, 153, 155, 156, 158, 158, 159, 161, 162, 162, 164, 166, 166, 168, 169, 169, 171, 173, 173, 175, 175, 177, 180, 180, 182, 182, 184, 184, 187, 187, 189, 189, 192, 192, 195, 195, 198, 198, 201, 201, 201, 204, 204, 208, 208, 208, 211, 211, 216, 216, 216, 220, 220, 220, 226, 226, 226, 226, 231, 231, 231, 231, 238, 238, 238, 238, 246, 246, 246, 246, 246, 255, 255, 255 };
	unsigned char* pSrc = srcData;
	unsigned char* pMsk = maskData;
	unsigned char* smoothMask = (unsigned char*)malloc(sizeof(unsigned char) * mHeight * mStride);
	f_TFastGaussFilter(maskData, mWidth, mHeight, mStride, smoothMask, 5.0f);
	float H = 0, S = 0, L = 0;
	RGB2HSV(R, G, B, &H, &S, &L);
	float h = 0, s = 0, l = 0;
	unsigned char r = 0, g = 0, b = 0;
	int k = ratio * 128 / 100;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int mx = i * mWidth / width;
			int my = j * mHeight / height;
			int pos = mx*4 + my * mStride;
			int alpha = smoothMask[pos];
			if(alpha > 128)
			{
				alpha = (alpha - 128) * 255 / 128;
				RGB2HSV(Map[pSrc[2]], Map[pSrc[1]], Map[pSrc[0]], &h, &s, &l);
				HSV2RGB(H, S, l, &r, &g, &b);
				r = CLIP3((r * k + pSrc[2] * (128 - k)) >> 7, 0, 255);
				g = CLIP3((g * k + pSrc[1] * (128 - k)) >> 7, 0, 255);
				b = CLIP3((b * k + pSrc[0] * (128 - k)) >> 7, 0, 255);
				pSrc[0] = CLIP3((b * alpha + pSrc[0] * (255 - alpha)) / 255, 0, 255);
				pSrc[1] = CLIP3((g * alpha + pSrc[1] * (255 - alpha)) / 255, 0, 255);
				pSrc[2] =  CLIP3((r * alpha + pSrc[2] * (255 - alpha)) / 255, 0, 255);
			}
			pSrc += 4;
		}
	}
	free(smoothMask);
	return ret;
};