#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_FBIM.h"


//Reference:Feature Based Image Metamorphosis
float lineGetU(int x, int y, int px, int py, int qx, int qy, int len)
{
	int xpx = x - px;
	int xpy = y - py;
	int qpx = qx - px;
	int qpy = qy - py;
	return (float)(xpx * qpx + xpy * qpy) / (len * len);
}
float lineGetV(int x, int y, int px, int py, int qx, int qy, int len)
{
	int xpx = x - px;
	int xpy = y - py;
	int qpx = qx - px;
	int qpy = qy - py;
	int per_qpx = qpy;
	int per_qpy = -qpx;
	return (float)(xpx * per_qpx + xpy * per_qpy) / len;
}
void getPointX(float u, float v, int px, int py, int qx, int qy, int len_dst, float *outPointX, float* outPointY)
{
	int qpx = qx - px;
	int qpy = qy - py;
	int per_qpx = qpy;
	int per_qpy = -qpx;
	*outPointX = px + u * (qx - px) + (v * per_qpx) / len_dst;
	*outPointY = py + u * (qy - py) + (v * per_qpy) / len_dst;
}
float getWeight(float x, float y, float px, float py, float qx, float qy, float len,  float a, float b, float p, float getU, float getV)
{
	float d = 0;
	float u = getU;//lineGetU(x, y, px, py, qx, qy, len);
	if(u > 1.0f)
		d = sqrt((x - qx) * (x - qx) + (y - qy) * (y - qy));
	else if(u < 0)
		d = sqrt((x - px) * (x - px) + (y - py) * (y - py));
	else
		d = fabs(getV);//abs(lineGetV(x, y, px, py,  qx, qy, len));
	return pow(pow(len,p) / (a + d), b);
}
/*************************************************************************
*Function: IDW
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*inputlinePoints: input feature line points of source image 
*outputlinePoints: output feature line points of dest image
*lineNum: count of lines
*Return: 0-OK,other failed
**************************************************************************/
int f_FBIM(unsigned char* srcData, int width, int height, int stride, int inputlinePoints[], int outputlinePoints[], int lineNum)
{
	int ret = 0;
	int *line_p_x   = (int*)malloc(sizeof(int) * lineNum);
	int *line_p_y   = (int*)malloc(sizeof(int) * lineNum);
	int *line_q_x   = (int*)malloc(sizeof(int) * lineNum);
	int *line_q_y   = (int*)malloc(sizeof(int) * lineNum);
	int *line_c_p_x = (int*)malloc(sizeof(int) * lineNum);
	int *line_c_p_y = (int*)malloc(sizeof(int) * lineNum);
	int *line_c_q_x = (int*)malloc(sizeof(int) * lineNum);
	int *line_c_q_y = (int*)malloc(sizeof(int) * lineNum);
	int index = 0;
	for(int i = 0; i < lineNum; i++)
	{
		line_p_x[i] = inputlinePoints[2 * index];
		line_p_y[i] = inputlinePoints[2 * index + 1];
		line_q_x[i] = inputlinePoints[2 * (index + 1)];
		line_q_y[i] = inputlinePoints[2 * (index + 1) + 1];

		line_c_p_x[i] = outputlinePoints[2 * index];
		line_c_p_y[i] = outputlinePoints[2 * index + 1];
		line_c_q_x[i] = outputlinePoints[2 * (index + 1)];
		line_c_q_y[i] = outputlinePoints[2 * (index + 1) + 1];
		index += 2;
	}
	int i, j, pos, nx, ny, k;
	unsigned char*  tempData = (unsigned char*)malloc(sizeof(unsigned char) *  height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) *  height * stride);
	float leftXSum_x, leftXSum_y, leftWeightSum, nu, nv, srcWeight, left_src_x, left_src_y, srcPx, srcPy, len_src, len_dst;
	float t1, t2;
	int pos_x_1, pos_y_1;
	float *MAP_Len_src = (float*)malloc(sizeof(float) * lineNum);
	float* MAP_Len_dst = (float*)malloc(sizeof(float) * lineNum);
	for(k = 0; k < lineNum; k++)
	{
		MAP_Len_src[k] = sqrt((float)(line_c_p_x[k] - line_c_q_x[k]) * (line_c_p_x[k] - line_c_q_x[k]) + (line_c_p_y[k] - line_c_q_y[k]) * (line_c_p_y[k] - line_c_q_y[k]));
		MAP_Len_dst[k] = sqrt((float)(line_p_x[k] - line_q_x[k]) * (line_p_x[k] - line_q_x[k]) + (line_p_y[k] - line_q_y[k]) * (line_p_y[k] - line_q_y[k]));
	}
	int* MAP_S = (int*)malloc(sizeof(int) * height);
	for(k = 0; k < height; k++)
	{
		MAP_S[k] = k * stride;
	}	
	float kw = 0, DX, DY;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			leftXSum_x = 0.0f;  
            leftXSum_y = 0.0f;  
            leftWeightSum = 0.0f;  
			for(k = 0; k < lineNum; k++)
			{
				len_src = MAP_Len_src[k];
				len_dst = MAP_Len_dst[k];
				nu = lineGetU(i, j, line_c_p_x[k], line_c_p_y[k],  line_c_q_x[k], line_c_q_y[k], (int)len_src);
				nv = lineGetV(i, j, line_c_p_x[k], line_c_p_y[k],  line_c_q_x[k], line_c_q_y[k], (int)len_src);
				getPointX(nu, nv, line_p_x[k], line_p_y[k], line_q_x[k], line_q_y[k],(int)len_dst, &srcPx, &srcPy);
				srcWeight = getWeight((float)i, (float)j, (float)line_c_p_x[k], (float)line_c_p_y[k],  (float)line_c_q_x[k], (float)line_c_q_y[k],(float)len_src,1.0f, 2.0f, 0.0f, nu, nv);
				leftXSum_x = leftXSum_x + srcPx * srcWeight;
				leftXSum_y = leftXSum_y + srcPy * srcWeight;
				leftWeightSum += srcWeight;
			}
			kw = (1.0f / leftWeightSum);  
			left_src_x = leftXSum_x * kw;  
            left_src_y = leftXSum_y * kw;  
			left_src_x = CLIP3(left_src_x, 0, width - 2);
			left_src_y = CLIP3(left_src_y, 0, height - 2); 
			nx = (int)left_src_x;
			ny = (int)left_src_y;
			pos = (i << 2) + MAP_S[j];

			DX = left_src_x - nx;
			DY = left_src_y - ny;
			pos_x_1 = nx + 1;
			pos_y_1 = ny + 1;

			t1 = (1.f  - DX) * tempData[(nx << 2) + MAP_S[ny]] + DX * tempData[(pos_x_1 << 2) + MAP_S[ny]];
			t2 = (1.f  - DX) * tempData[(nx << 2) + MAP_S[pos_y_1]] + DX * tempData[(pos_x_1 << 2) + MAP_S[pos_y_1]];
			t1 = (1.f - DY) * t1 + DY * t2;

			srcData[pos] = (unsigned char)CLIP3(t1, 0, 255);
			t1 = (1.f  - DX) * tempData[(nx << 2) + 1 + MAP_S[ny]] + DX * tempData[(pos_x_1 << 2) + 1 + MAP_S[ny]];
			t2 = (1.f  - DX) * tempData[(nx << 2) + 1 + MAP_S[pos_y_1]] + DX * tempData[(pos_x_1 << 2) + 1 + MAP_S[pos_y_1]];
			t1 = (1.f  - DY) * t1 + DY * t2;
			srcData[pos + 1] = (unsigned char)CLIP3(t1, 0, 255);				
			t1 = (1.f  - DX) * tempData[(nx << 2) + 2 + MAP_S[ny]] + DX * tempData[(pos_x_1 << 2) + 2 + MAP_S[ny]];
			t2 = (1.f  - DX) * tempData[(nx << 2) + 2 + MAP_S[pos_y_1]]  + DX * tempData[(pos_x_1 << 2) + 2 + MAP_S[pos_y_1]];
			t1 = (1.f  - DY) * t1 + DY * t2;
			srcData[pos + 2] = (unsigned char)CLIP3(t1, 0, 255);       
		}
	}
	free(MAP_S);
	free(tempData);
	free(line_p_x  );
	free(line_p_y  );
	free(line_q_x  );
	free(line_q_y  );
	free(line_c_p_x);
	free(line_c_p_y);
	free(line_c_q_x);
	free(line_c_q_y);
	free(MAP_Len_src);
	free(MAP_Len_dst);
	return ret;
};