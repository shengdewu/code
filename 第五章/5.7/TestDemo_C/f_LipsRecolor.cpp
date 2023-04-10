#include"f_MakeupBase.h"
#include"f_LipsRecolor.h"
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include"Commen.h"
#include"f_AffineTransfrom.h"

inline void computeOutRectInfo(int points[], int pointNum, int outPoints[2 * 8])
{
	int minx = 10000, maxx = 0, miny = 10000, maxy = 0;
	for(int i = 0; i < pointNum; i++)
	{
		minx = MIN2(minx, points[2 * i]);
		miny = MIN2(miny, points[2 * i + 1]);
		maxx = MAX2(maxx, points[2 * i]);
		maxy = MAX2(maxy, points[2 * i + 1]);
	}
	int w = maxx - minx;
	int h = maxy - miny;
	minx = minx - w / 5;
	miny = miny - h / 5;
	maxx = maxx + w / 5;
	maxy = maxy + h / 5;
	outPoints[0] = minx;
	outPoints[1] = miny;
	outPoints[2] = minx;
	outPoints[3] = miny + h / 2;
	outPoints[4] = minx;
	outPoints[5] = maxy;
	outPoints[6] = minx + w / 2;
	outPoints[7] = maxy;
	outPoints[8] = maxx;
	outPoints[9] = maxy;
	outPoints[10] = maxx;
	outPoints[11] = miny + h / 2;
	outPoints[12] = maxx;
	outPoints[13] = miny;
	outPoints[14] = minx + w / 2;
	outPoints[15] = miny;
};
//compute squares of triangle
inline float GetTriangleSquar(float pt0_x, float pt0_y, float pt1_x, float pt1_y, float pt2_x, float pt2_y)  
{  
    float AB_x, AB_y, BC_x, BC_y;      
    AB_x = pt1_x - pt0_x;     
    AB_y = pt1_y - pt0_y;     
    BC_x = pt2_x - pt1_x;     
    BC_y = pt2_y - pt1_y;       
    return abs((AB_x * BC_y - AB_y * BC_x)) * 0.5f;      
}  
//judge the point is in the triangle or not.
inline bool JudgePointInTriangleOrNot(float curPoint_x, float curPoint_y, float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{	  
    float ABS_FLOAT_0 = 0.0001;
	float SABC, SADB, SBDC, SADC;  
    SABC = GetTriangleSquar(Ax,Ay, Bx, By, Cx, Cy);  
    SADB = GetTriangleSquar(Ax,Ay, curPoint_x, curPoint_y, Bx, By);  
    SBDC = GetTriangleSquar(Bx,By, curPoint_x, curPoint_y, Cx, Cy);  
    SADC = GetTriangleSquar(Ax,Ay, curPoint_x, curPoint_y, Cx, Cy);   
    float SumSuqar = SADB + SBDC + SADC;  
    if ((-ABS_FLOAT_0 < (SABC - SumSuqar)) && ((SABC - SumSuqar) < ABS_FLOAT_0))  
    {  
        return true;  
    }  
    else  
    {  
        return false;  
    }  
}
/*************************************************************************
*Function: Lips recolor
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*srcFacePoints: 101 face points.
*mskData: Lips image mask buffer with format bgra32
*mWidth: width of mask image 
*mHeight: height of mask image
*mStride: Stride of mask image
*maskKeyPoints: 28 key points of lips mask.
*isLeft: left or right face.
*ratio: intensity of effect, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_LipsRecolor(unsigned char* srcData, int width, int height, int stride, int srcFacePoints[202], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 28], unsigned char* lipsColorMap,  int ratio)
{
	int ret = 0;
	////compute 28 points of lips for srcData
	int srcLipsPoints[2 * 28] = {0};
	for(int i = 0; i < 2 * 20; i++)
	{
		srcLipsPoints[i] = srcFacePoints[2 * 75 + i];
	}
	int srcRectInfo[2 * 8] = {0};
	computeOutRectInfo(srcLipsPoints, 20, srcRectInfo);
	for(int i = 0; i < 2 * 8; i++)
	{
		srcLipsPoints[2 * 20 + i] = srcRectInfo[i];
	}
	///42 TRIANGLES
	const int TRIANGLE_NUM = 42;
	int TRIANGLE_INDEX[3 * TRIANGLE_NUM] = {20, 21, 1, 1, 21, 0, 0, 21, 11, 11, 21, 22, 11, 22, 23, 11, 23, 10, 10,23, 9, 9, 23, 8, 8, 23, 7, 7, 23, 24, 7, 24, 25, 7, 25, 6, 6, 25, 5, 5, 25, 26, 5, 26, 27, 5, 27, 4, 4, 27, 3, 3, 27, 2, 2, 27, 1, 1, 27, 20, 
	0, 11, 19, 19, 11, 10, 19, 10, 18, 18, 10, 9, 18, 9, 8, 18, 8, 17, 17, 8, 7, 7, 6, 17, 6, 5, 15, 15, 5, 4, 15, 4, 14, 14, 4, 3, 14, 3, 2, 14, 2, 13, 13, 2, 1, 13, 1, 0,
	0, 19, 13, 13, 19, 18, 13, 18, 14, 14, 18, 15, 15, 18, 17, 15, 17, 6};
	unsigned char* label = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	memset(label, 0, sizeof(unsigned char) * width * height);
	for(int i = 0; i < TRIANGLE_NUM; i++)
	{
		float H[6] = {0};
		f_AffinetransformMetrixCompute(maskKeyPoints[2 * TRIANGLE_INDEX[3 * i]],maskKeyPoints[2 * TRIANGLE_INDEX[3 * i] + 1], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 1]],maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 1] + 1], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 2]], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 2] + 1], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i]], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i] + 1], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1]], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1] + 1],srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2]],srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2] + 1], H);
		int px = MIN2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i]], MIN2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1]], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2]]));
	    int py = MIN2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i] + 1], MIN2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1] + 1], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2] + 1]));
	    int mw = (int)(MAX2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i]],MAX2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1]], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2]])) - px);
	    int mh = (int)(MAX2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i] + 1],MAX2(srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 1] + 1], srcLipsPoints[2 * TRIANGLE_INDEX[3 * i + 2] + 1])) - py);
		px = px - 1;
		py = py - 1;
		mw = mw + 2;
		mh = mh + 2;
		for(int n = 0; n < mh; n++)
		{
			for(int m = 0; m < mw; m++)
			{
				int cx = m + px;
				int cy = n + py;
				if(label[cx + cy * width] > 0)
					continue;
				int pos = cx * 4 + cy * stride;
				float x0 = (H[0] * cx + H[1] * cy + H[2]);
	            float y0 = (H[3] * cx + H[4] * cy + H[5]);
				bool isIn = JudgePointInTriangleOrNot(x0, y0, maskKeyPoints[2 * TRIANGLE_INDEX[3 * i]],maskKeyPoints[2 * TRIANGLE_INDEX[3 * i] + 1], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 1]],maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 1] + 1], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 2]], maskKeyPoints[2 * TRIANGLE_INDEX[3 * i + 2] + 1]); 
			    if(isIn)
				{
					//get alpha for blend
					int msk_b = mskData[(int)x0 * 4 + (int)y0 * mStride];
					int msk_g = mskData[(int)x0 * 4 + (int)y0 * mStride + 1];
					int msk_r = mskData[(int)x0 * 4 + (int)y0 * mStride + 2];
					int b = srcData[pos];
					int g = srcData[pos + 1];
					int r = srcData[pos + 2];
					int gray = (msk_b + msk_g + msk_r) / 3;
					//lips recolored using 512 lut
					int k  =  (b  >>  2);
                    int nx  =  (int)(r  >>  2)  +  ((k  -  ((k  >>  3)  <<  3))  <<  6);
                    int ny  =  (int)(((b  >>  5)  <<  6)  +  (g  >>  2));
                    int mpos  =  (nx * 4)  +  (ny * 512 * 4);
					int mb = lipsColorMap[mpos];
					int mg = lipsColorMap[mpos + 1];
					int mr = lipsColorMap[mpos + 2];
					//lips and color blend by alpha
					mb = CLIP3((b * (255 - gray) + mb * gray) / 255, 0, 255);
					mg = CLIP3((g * (255 - gray) + mg * gray) / 255, 0, 255);
					mr = CLIP3((r * (255 - gray) + mr * gray) / 255, 0, 255);
					//color intensity blending
					srcData[pos] = (mb * ratio + b * (100 - ratio)) / 100;
					srcData[pos + 1] = (mg * ratio + g * (100 - ratio)) / 100;
					srcData[pos + 2] = (mr * ratio + r * (100 - ratio)) / 100;
					label[cx + cy * width] = 255;
				}
				
			}
		}
	}
	free(label);
	return ret;
};
