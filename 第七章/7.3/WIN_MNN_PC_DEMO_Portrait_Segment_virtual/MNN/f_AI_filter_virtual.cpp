#include"f_AI_filter_virtual.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

int f_TMosaic(unsigned char* srcData, int width, int height, int stride, int size)
{
	if(NULL == srcData)
		return 1;
	if(size == 0)
		return 0;
    int i, j, pos;
	int sum_b = 0, sum_g = 0, sum_r = 0;
	int count = 0;
	int num_r = (width % size == 0) ? (width / size) : (int)(width / size + 1);
	int num_c = (height % size == 0) ? (height / size) : (int)(height / size + 1);
	int m, n;
	int t = 0, k = 0;
	int* map = (int*)malloc(sizeof(int) * height);
	if(NULL == map)
		return 1;
	for(i = 0; i < height; i ++)
	{
		map[i] = i * stride;
	}
	for(j = 0; j < num_c; j++)
	{
		for(i = 0; i < num_r; i++)
		{
			for(n = 0; n < size; n++)
			{
				for(m = 0; m < size; m++)
				{
					t = i * size + m;
					k = j * size + n;
					if(t >= width || k >= height)
						continue;
					else
					{
					    pos = (t << 2) + map[k];
						sum_b += srcData[pos];
						sum_g += srcData[pos + 1];
						sum_r += srcData[pos + 2];
						count++;
					}
				}
			}
			sum_b = sum_b / count;
			sum_g = sum_g / count;
			sum_r = sum_r / count;
			for(n = 0; n < size; n++)
			{
				for(m = 0; m < size; m++)
				{
					t = i * size + m;
					k = j * size + n;
					if(t >= width || k >= height)
						continue;
					else
					{
					    pos = (t << 2) + map[k];
						srcData[pos]     = sum_b;
						srcData[pos + 1] = sum_g;
						srcData[pos + 2] = sum_r;
					}
				}
			}
			sum_b = sum_g = sum_r = 0;
			count = 0;
		}
	}
	free(map);
	return 0;
}
   int f_TDiffusion(unsigned char* srcData,int width, int height,int stride,int intensity)
   {
	   if(srcData == NULL)
		   return 0;
	   if(intensity == 0)
		   return 0;
	   int i, j, pos_t;
	   int m, n, kx, ky;
	   kx = width - 1;
	   ky = height - 1;
	   unsigned char* pSrc = srcData;
	   int offset = stride - (width << 2);
	   int* map = (int*)malloc(sizeof(int) * height);
	   if(NULL == map)
		   return 1;
	   for(i = 0; i <= ky; i++)
	   {
		   map[i] = i * stride;
	   }
	   for(j = 0; j < height;j++)
	   {
		   for(i = 0; i < width; i++)
		   {
			   m = (int)(rand()%intensity);
			   n = (int)(rand()%intensity);
			   m = MIN2(i + m, kx);
			   n = MIN2(j + n, ky);
			   pos_t = (m << 2) + map[n];
			   pSrc[0] = srcData[pos_t];
			   pSrc[1] = srcData[pos_t + 1];
			   pSrc[2] = srcData[pos_t + 2];
			   pSrc += 4;
		   }
		   pSrc += offset;
	   }
	   free(map);
	   return 0;
   };

int f_TZoomBlur(unsigned char* srcData, int width, int height ,int stride, int cenX, int cenY, int sampleRadius, int amount)
{
	if(NULL == srcData)
		return 0;
	if(sampleRadius == 0)
		return 0;
	//sampleRadius = amount * 255 / 200;
	sampleRadius = CLIP3(sampleRadius, 1, 255);
	amount = CLIP3(amount, 1, 200);
	unsigned char* dstData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	int* map = (int*)malloc(sizeof(int) * height);
	if(NULL == dstData || NULL == map)
        return 1;
	memcpy(dstData, srcData, sizeof(unsigned char) * height * stride);
	int r, g, b, a;
	int Fcx, Fcy, Fx, Fy;
	int TempFy, V, U, Speed, SpeedC;
	Fcx = 65535 * cenX;
	Fcy = 65535 * cenY;    
	int i, j, k;
	for(i = 0; i < height; i ++)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
		map[i] = i * stride;
	}
	for(j = 0; j < height; j++)
	{
        TempFy = (j << 16) - j  - Fcy;
		Speed = map[j];
		for(i = 0; i < width; i++)
		{
			Fx = (i << 16) - i - Fcx;
			Fy = TempFy;
			r = 0; g = 0; b = 0; a = 0;
			for(k = 1; k < sampleRadius; k++)
			{
				Fx = Fx - (((Fx >> 4) * amount) >> 10);
                Fy = Fy - (((Fy >> 4) * amount) >> 10);
                U = (Fx + Fcx) >> 16;
                V = (Fy + Fcy) >> 16;
				SpeedC = map[V] + (U << 2);
				b = b + dstData[SpeedC];
				g = g + dstData[SpeedC + 1];
				r = r + dstData[SpeedC + 2];
				a = a + dstData[SpeedC + 3];
			}
			srcData[Speed] = b / (sampleRadius - 1);
			srcData[Speed + 1] = g / (sampleRadius - 1);
			srcData[Speed + 2] = r / (sampleRadius - 1);
			srcData[Speed + 3] = a / (sampleRadius - 1);
			Speed = Speed + 4;
		}
	}
	free(dstData);
	free(map);
	return 0;
};
int f_TOilpaint(unsigned char* pSrc, int width, int height, int stride,int radius, int smooth)
{
    unsigned char* srcData = (unsigned char *)malloc(height*stride);
	int* gray_bt = (int*)malloc(sizeof(int)*smooth);
    int* r_bt = (int*)malloc(sizeof(int)*smooth);
    int* g_bt = (int*)malloc(sizeof(int)*smooth);
    int* b_bt = (int*)malloc(sizeof(int)*smooth);
    int* gray_bt_src = (int*)malloc(sizeof(int)*smooth);
    int* r_bt_src = (int*)malloc(sizeof(int)*smooth);
    int* g_bt_src = (int*)malloc(sizeof(int)*smooth);
    int* b_bt_src = (int*)malloc(sizeof(int)*smooth);
	if(NULL == srcData || NULL == gray_bt || NULL == r_bt || NULL == g_bt || NULL == b_bt || NULL == gray_bt_src || NULL == r_bt_src || NULL == g_bt_src || NULL == b_bt_src)
		return 1;
    memcpy(srcData, pSrc, height*stride);
    smooth = smooth < 1 ? 1 : smooth;
    int i, j, gray, k, r, g, b, m, n;
    int unit = 4;
    int bt_index = 0, max = 0, maxindex = 0;
    bool frist = true;
    memset(gray_bt_src,0,smooth*sizeof(int));
    memset(b_bt_src,0,smooth*sizeof(int));
    memset(g_bt_src,0,smooth*sizeof(int));
    memset(r_bt_src,0,smooth*sizeof(int));
    int pos = 0;
    int len = smooth*sizeof(int);
    for (j = 0; j < height; j++)
    {
        if (frist)
        {
            for (m = -radius; m <= radius; m++)
            {
                for (n = -radius; n <= radius; n++)
                {
                    pos = abs(n) * unit + abs(m) * stride;
                    b = srcData[pos++];
                    g = srcData[pos++];
                    r = srcData[pos];
                    gray = (b + g + r) / 3;
                    bt_index = gray * smooth >> 8;
                    gray_bt_src[bt_index]++;
                    b_bt_src[bt_index] += b;
                    g_bt_src[bt_index] += g;
                    r_bt_src[bt_index] += r;
                }
            }
            memcpy(gray_bt,gray_bt_src,len);
            memcpy(b_bt,b_bt_src,len);
            memcpy(g_bt,g_bt_src,len);
            memcpy(r_bt,r_bt_src,len);
            max = 0;
            maxindex = 0;
            for (k = 0; k < smooth; k++)
            {
                if (max < gray_bt[k])
                {
                    max = gray_bt[k];
                    maxindex = k;
                }
            }
            pos = j * stride;
            pSrc[pos++] = (b_bt[maxindex] / max);
            pSrc[pos++] = (g_bt[maxindex] / max);
            pSrc[pos] = (r_bt[maxindex] / max);
            frist = false;
        }
        else
        {
            for (m = -radius; m <= radius; m++)
            {
                pos = abs(m) * unit + abs(j - radius - 1) * stride;
                b = srcData[pos++];
                g = srcData[pos++];
                r = srcData[pos];
                gray = (b + g + r) / 3;
                bt_index = gray * smooth >> 8;
                gray_bt_src[bt_index]--;
                b_bt_src[bt_index] -= b;
                g_bt_src[bt_index] -= g;
                r_bt_src[bt_index] -= r;

                pos = abs(m) * unit + abs(j + radius) % height * stride;
                b = srcData[pos++];
                g = srcData[pos++];
                r = srcData[pos];
                gray = (b + g + r) / 3;
                bt_index = gray * smooth >> 8;
                gray_bt_src[bt_index]++;
                b_bt_src[bt_index] += b;
                g_bt_src[bt_index] += g;
                r_bt_src[bt_index] += r;
            }
            memcpy(gray_bt,gray_bt_src,len);
            memcpy(b_bt,b_bt_src,len);
            memcpy(g_bt,g_bt_src,len);
            memcpy(r_bt,r_bt_src,len);
        }
        for (i = 1; i < width; i++)
        {
            for (m = -radius; m <= radius; m++)
            {
                pos = abs(i - radius - 1) * unit + abs(j + m) % height * stride;
                b = srcData[pos++];
                g = srcData[pos++];
                r = srcData[pos];
                gray = (b + g + r) / 3;
                bt_index = gray * smooth >> 8;
                gray_bt[bt_index]--;
                b_bt[bt_index] -= b;
                g_bt[bt_index] -= g;
                r_bt[bt_index] -= r;

                pos = abs(i + radius) % width * unit + abs(j + m) % height * stride;
                b = srcData[pos++];
                g = srcData[pos++];
                r = srcData[pos];
                gray = (b + g + r) / 3;
                bt_index = gray * smooth >> 8;
                gray_bt[bt_index]++;
                b_bt[bt_index] += b;
                g_bt[bt_index] += g;
                r_bt[bt_index] += r;
            }
            max = 0;
            maxindex = 0;
            for (k = 0; k < smooth; k++)
            {
                if (max < gray_bt[k])
                {
                    max = gray_bt[k];
                    maxindex = k;
                }
            }
            pos = i * unit + j * stride;
            pSrc[pos++] = (b_bt[maxindex] / max);
            pSrc[pos++] = (g_bt[maxindex] / max);
            pSrc[pos] = (r_bt[maxindex] / max);
        }             
    }
    free(gray_bt_src);
    free(b_bt_src);
    free(g_bt_src);
    free(r_bt_src);
    free(gray_bt);
    free(b_bt);
    free(g_bt);
    free(r_bt);
    free(srcData);
    return 0;
};
/************************************************************
*Function:  f_FocusVirtualFilter
*Description: Virtual filter
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*mask: the person mask of image.
*intensity: intensity of effect, [0,100].
*Return :0-OK,or failed    
************************************************************/
int f_FocusVirtualFilter(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity)
{
	int ret = 0;
	int i, j;
	unsigned char* gaussData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	int radius = intensity * 20 / 100;
	memcpy(gaussData, srcData, sizeof(unsigned char) * stride * height);
	int k, nk;
	int dis;
	int max = sqrt((float)width * width + height * height) / 2;
	int cx = width / 2;
	int cy = height / 2;
	int nx, ny, pos, r, g, b;
	k = 7;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			pos = i * 4 + j * stride;
			if(mask[pos] < 128)
			{
			    radius = ((int)sqrt((float)(i - cx) * (i - cx) + (j - cy) * (j - cy)) * k / max);
				float sigma = 3 * radius / 3.0f;
				float sumr = 0, sumg = 0, sumb = 0;
				float w = 0, sw = 0;
			    for(int n = -radius; n < radius; n++)
				{
					for(int m = -radius; m < radius; m++)
					{
						nx = CLIP3(i + m, 0, width - 1);
						ny = CLIP3(j + n, 0, height - 1);
						w = exp(-(float)(m*m+n*n)/(2*sigma*sigma));
						sw += w;
						sumb += gaussData[nx * 4 + ny * stride] * w;
						sumg += gaussData[nx * 4 + ny * stride + 1] * w;
						sumr += gaussData[nx * 4 + ny * stride + 2] * w;
					}
				}
				if(sumb!=0)
			        srcData[pos    ] = CLIP3((int)(sumb / sw), 0, 255);
				if(sumg!=0)
			        srcData[pos + 1] = CLIP3((int)(sumg / sw), 0, 255);
				if(sumr!=0)
			        srcData[pos + 2] = CLIP3((int)(sumr / sw), 0, 255);
			}
		}
	}
	free(gaussData);
	return ret;
};
int f_PortraitEffect(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity, int effectId)
{
	int ret = 0;
	int i, j;
	unsigned char* pSrc = srcData;
	unsigned char* pMask = mask;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	memcpy(tempData, srcData, sizeof(unsigned char) * stride * height);
	unsigned char* pEffect = tempData;
	int k = 0;
	switch (effectId)
	{
	case 0://Mosaic
		f_TMosaic(tempData, width, height, stride, 20);
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				k = (pMask[0] + pMask[1] + pMask[2]) / 3;
				if (k < 255)
				{
					int b = (pSrc[0] * k + pEffect[0] * (255 - k)) / 255;
					pSrc[0] = CLIP3((b * intensity + pSrc[0] * (100 - intensity)) / 100, 0, 255);
					int g = (pSrc[1] * k + pEffect[1] * (255 - k)) / 255;
					pSrc[1] = CLIP3((g * intensity + pSrc[1] * (100 - intensity)) / 100, 0, 255);
					int r = (pSrc[2] * k + pEffect[2] * (255 - k)) / 255;
					pSrc[2] = CLIP3((r * intensity + pSrc[2] * (100 - intensity)) / 100, 0, 255);
				}
				pSrc += 4;
				pMask += 4;
				pEffect += 4;
			}
		}
		break;
	case 1://Diffusion
		f_TDiffusion(tempData, width, height, stride, 10);
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				k = (pMask[0] + pMask[1] + pMask[2]) / 3;
				if (k < 255)
				{
					int b = (pSrc[0] * k + pEffect[0] * (255 - k)) / 255;
					pSrc[0] = CLIP3((b * intensity + pSrc[0] * (100 - intensity)) / 100, 0, 255);
					int g = (pSrc[1] * k + pEffect[1] * (255 - k)) / 255;
					pSrc[1] = CLIP3((g * intensity + pSrc[1] * (100 - intensity)) / 100, 0, 255);
					int r = (pSrc[2] * k + pEffect[2] * (255 - k)) / 255;
					pSrc[2] = CLIP3((r * intensity + pSrc[2] * (100 - intensity)) / 100, 0, 255);
				}
				pSrc += 4;
				pMask += 4;
				pEffect += 4;
			}
		}
		break;
	case 2://ZoomBlur
		f_TZoomBlur(tempData, width, height, stride, width / 2, height / 2, 20, 50);
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				k = (pMask[0] + pMask[1] + pMask[2]) / 3;
				if (k < 255)
				{
					int b = (pSrc[0] * k + pEffect[0] * (255 - k)) / 255;
					pSrc[0] = CLIP3((b * intensity + pSrc[0] * (100 - intensity)) / 100, 0, 255);
					int g = (pSrc[1] * k + pEffect[1] * (255 - k)) / 255;
					pSrc[1] = CLIP3((g * intensity + pSrc[1] * (100 - intensity)) / 100, 0, 255);
					int r = (pSrc[2] * k + pEffect[2] * (255 - k)) / 255;
					pSrc[2] = CLIP3((r * intensity + pSrc[2] * (100 - intensity)) / 100, 0, 255);
				}
				pSrc += 4;
				pMask += 4;
				pEffect += 4;
			}
		}
		break;

	case 3://Oilpaint
		f_TOilpaint(tempData, width, height, stride, 15, 50);
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				k = (pMask[0] + pMask[1] + pMask[2]) / 3;
				if (k < 255)
				{
					int b = (pSrc[0] * k + pEffect[0] * (255 - k)) / 255;
					pSrc[0] = CLIP3((b * intensity + pSrc[0] * (100 - intensity)) / 100, 0, 255);
					int g = (pSrc[1] * k + pEffect[1] * (255 - k)) / 255;
					pSrc[1] = CLIP3((g * intensity + pSrc[1] * (100 - intensity)) / 100, 0, 255);
					int r = (pSrc[2] * k + pEffect[2] * (255 - k)) / 255;
					pSrc[2] = CLIP3((r * intensity + pSrc[2] * (100 - intensity)) / 100, 0, 255);
				}
				pSrc += 4;
				pMask += 4;
				pEffect += 4;
			}
		}
		break;
	default:
		break;
	}
	free(tempData);
	return ret;
}
