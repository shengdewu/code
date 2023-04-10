#include"f_Canny.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"


//单通道灰度化
static int f_GrayOneChannel(unsigned char* srcData, unsigned char* grayData, int width, int height, int stride)
{
	int ret = 0;
	int i, j, gray, offset;
	offset = stride - (width * 4);
	unsigned char* pSrc = srcData;
	unsigned char* pGray = grayData;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			gray = (pSrc[2] + pSrc[1] + pSrc[0]) / 3;
			*pGray = gray;
			pSrc += 4;
			pGray ++;
		}
		pSrc += offset;
	}
	return ret;
};
//梯度相位角获取  
static void GetGradientDegree(unsigned char* srcBytes, int width, int height, float gradient[], unsigned char degree[], float* GradientMax)  
{  
    float gx, gy;  
    int temp, pos;  
    float div;  
	float PI = 3.1415926f;
	float t = 180.0f/PI;
    for (int j = 1; j < height - 1; j++)  
    {  
        for (int i = 1; i < width - 1; i++)  
        {  
			pos = i + j * width;
            gx = srcBytes[pos + 1 - width] + srcBytes[pos + 1] + srcBytes[pos + 1] + srcBytes[pos + 1 + width] - srcBytes[pos - 1 - width] - (srcBytes[pos - 1] + srcBytes[pos - 1]) - srcBytes[pos - 1 + width];  
            gy = srcBytes[pos - 1 - width] + srcBytes[pos - width] + srcBytes[pos - width] + srcBytes[pos + 1 - width] - srcBytes[pos - 1 + width] - (srcBytes[pos + width] + srcBytes[pos + width]) - srcBytes[pos + 1 + width];  
            gradient[pos] = (float)sqrt((float)(gx * gx + gy * gy));  
            if (*GradientMax < gradient[pos])  
            {  
                *GradientMax = gradient[pos];  
            }  
            if (gx == 0)  
            {  
                temp = (gy == 0) ? 0 : 90;  
            }  
            else  
            {  
                div = gy / gx;  
                if (div < 0)  
                {  
                    temp = (int)(180 - atan(-div) * t);  
                }  
                else  
                {  
                    temp = (int)(atan(div) * t);  
                }  
                if (temp < 22.5f)  
                {  
                    temp = 0;  
                }  
                else if (temp < 67.5f)  
                {  
                    temp = 45;  
                }  
                else if (temp < 112.5f)  
                {  
                    temp = 90;  
                }  
                else if (temp < 157.5f)  
                {  
                    temp = 135;  
                }  
                else  
                    temp = 0;  
            }  
            degree[pos] = temp;  
        }  
    }  
	
} ; 
//非极大值抑制  
static void NonMaxMini(unsigned char* srcBytes, int width, int height, float gradient[], float GradientMax, unsigned char degree[])  
{  
    float leftPixel = 0, rightPixel = 0;
	int pos;
    for (int j = 1; j < height - 1; j++)  
    {  
        for (int i = 1; i < width - 1; i++)  
        {  
			pos = i + j * width;
            switch (degree[pos])  
            {  
                case 0:  
                    leftPixel = gradient[pos - 1];  
                    rightPixel = gradient[pos + 1];  
                    break;  
                case 45:  
                    leftPixel = gradient[pos - 1 + width];  
                    rightPixel = gradient[pos + 1 - width];  
                    break;  
                case 90:  
                    leftPixel = gradient[pos + width];  
                    rightPixel = gradient[pos - width];  
                    break;  
                case 135:  
                    leftPixel = gradient[pos + 1 + width];  
                    rightPixel = gradient[pos - 1 - width];  
                    break;  
                default:  
                    break;  
            }  
            if ((gradient[pos] < leftPixel) || (gradient[pos] < rightPixel))  
            {  
                srcBytes[pos] = 0;  
            }  
            else  
            {  
                srcBytes[pos] = (int)(255.0f * gradient[pos] / GradientMax);  
            }  
        }  
    }  
};  
//双阈值边缘判断  
static void TwoThreshouldJudge(unsigned char* srcBytes, int width, int height, int highThreshold, int lowThreshould)  
{  
	int pos = 0;
    for (int j = 1; j < height - 1; j++)  
    {  
        for (int i = 1; i < width - 1; i++)  
        {  
			pos = i + j * width;
            if (srcBytes[pos] > highThreshold)  
            {  
                srcBytes[pos] = 255;  
            }  
            else if (srcBytes[pos] < lowThreshould)  
            {  
                srcBytes[pos] = 0;  
            }  
            else  
            {  
                if (srcBytes[pos - 1 - width] < highThreshold && srcBytes[pos - width] < highThreshold && srcBytes[pos + 1 - width] < highThreshold && srcBytes[pos - 1] < highThreshold  
                    && srcBytes[pos + 1] < highThreshold && srcBytes[pos - 1 + width] < highThreshold && srcBytes[pos + width] < highThreshold && srcBytes[pos + 1 + width] < highThreshold)  
                {  
                    srcBytes[pos] = 0;  
                }  
                else  
                    srcBytes[pos] = 255;  
            }  
        }  
    }  
}; 
/************************************************************
*Function:  Canny edge detection
*Description: Canny edge detection
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*highThreshold:[0,255]
*lowThreshold: [0,255],default 0.4 * highThreshold
*Return :0-OK,or failed    
************************************************************/
int f_CannyEdgedetection(unsigned char* srcData, int width ,int height, int stride, int highThreshold,int lowThreshold)
{
	int ret = 0;
	int i, j, offset, pos, temp, size;
	unsigned char* pSrc = srcData;
	size = width * height;
	unsigned char* grayData = (unsigned char*)malloc(sizeof(unsigned char) * size);
	memset(grayData, 0, sizeof(unsigned char) * size);
	offset = stride - width * 4;
	//gray
	f_GrayOneChannel(srcData, grayData, width, height, stride);
	//gauss fiter
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			pos = i + j * width;
			if(i == 0 || j == 0 || i == width - 1 || j == height - 1)
			{
				grayData[pos] = 0;
			}
			else
			{
			    temp = ((grayData[pos] << 2) + grayData[pos - width - 1] + grayData[pos + 1 - width] + grayData[pos - 1 + width] + grayData[pos + 1 + width] + grayData[pos - width] + grayData[pos - width] + grayData[pos - 1] + grayData[pos - 1] + grayData[pos + width] + grayData[pos + width] + grayData[pos + 1] + grayData[pos + 1]) >> 4;  
		        grayData[pos] = temp;
			}
		}
	}
	//gradient
	float* gradient = (float*)malloc(sizeof(float) * size);
	memset(gradient, 0, sizeof(float) * size);
	unsigned char* degree = (unsigned char*)malloc(sizeof(unsigned char) * size);
	memset(degree, 0, sizeof(unsigned char) * size);
	float GradientMax = 0;
	GetGradientDegree(grayData, width, height, gradient, degree, &GradientMax);
	//none max value 
	NonMaxMini(grayData, width, height, gradient,GradientMax,degree);
	//two threshold judgement
	TwoThreshouldJudge(grayData,width, height,highThreshold,lowThreshold);
	//recovery
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			pSrc[0] = pSrc[1] = pSrc[2] = grayData[i + j * width];
			pSrc += 4;
		}
		pSrc += offset;
	}
	free(grayData);
	free(gradient);
	free(degree);
	return ret;
};