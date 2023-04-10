#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_IDW.h"


//Reference:Image Warping with Scattered Data Interpolation
int f_IDW(unsigned char* srcData, int width, int height, int stride, int inputPoints[], int outputPoints[], int pointNum)
{
	int ret = 0; 
	unsigned char* pSrc = srcData;
	int aa, bb, cc, dd, pos, pos1, xx, yy;
    double r1, r2, r3;
	unsigned char *pSrcL1;
    unsigned char *pSrcL2;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char)* height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	unsigned char* p = tempData;
	double w = 0, x_in, y_in, sumw, v;
	double u = 1;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			x_in = 0, y_in = 0;	
			//F function compute
			sumw = 0;
			for(int k = 0; k < pointNum; k++)
			{
				int t = (k << 1);
				v = 1.0 / (pow((double)((i - inputPoints[t]) * (i - inputPoints[t]) + (j - inputPoints[t + 1]) * (j - inputPoints[t + 1])),u));
				sumw += v;
				if(i == inputPoints[t] && j == inputPoints[t + 1])
					w = 1.0;
				else
				    w = v;		
				x_in += w * (outputPoints[t] + i - inputPoints[t]);
				y_in += w * (outputPoints[t + 1] + j - inputPoints[t + 1]);
			}
			w = 1.0 / sumw;
			x_in = x_in * w;
			y_in = y_in * w;
			//interpolation
			x_in = CLIP3(x_in, 0, width - 1);
			y_in = CLIP3(y_in, 0, height - 1);
			xx = (int)x_in;
            yy = (int)y_in; 
			pSrcL1 = p + yy * stride;
            pSrcL2 = p + CLIP3((yy + 1), 0, height - 1) * stride;
			pos = (xx << 2);
            aa = pSrcL1[pos];
            bb = pSrcL1[pos + 4];
            cc = pSrcL2[pos];
            dd = pSrcL2[pos + 4];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[0]=(unsigned char)(CLIP3(r3, 0, 255));//B
            aa = pSrcL1[pos + 1];
            bb = pSrcL1[pos + 4 +1];
            cc = pSrcL2[pos + 1];
            dd = pSrcL2[pos + 4 + 1];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[1]=(unsigned char)(CLIP3(r3, 0, 255));//G
            aa = pSrcL1[pos + 2];
            bb = pSrcL1[pos + 4 + 2];
            cc = pSrcL2[pos + 2];
            dd = pSrcL2[pos + 4 + 2];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[2]=(unsigned char)(CLIP3(r3, 0, 255));//R
            aa = pSrcL1[pos + 3];
            bb = pSrcL1[pos + 4 + 3];
            cc = pSrcL2[pos + 3];
            dd = pSrcL2[pos + 4 + 3];
            r1=aa + (bb - aa) * (x_in - xx);
            r2=cc + (dd - cc) * (x_in - xx);
            r3=r1 + (r2 - r1) * (y_in - yy);
            pSrc[3]=(unsigned char)(CLIP3(r3, 0, 255));//A
		   pSrc += 4;
		}
	}
	free(tempData);
	return ret;
};
