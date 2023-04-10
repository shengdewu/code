
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: MLS image warp algorithm
Reference:Image Deformation Using Moving Least Squares
*************************************************************************/
#ifndef __T_MLSIMAGEWARP__
#define __T_MLSIMAGEWARP__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
struct PointD
{
	double x;
	double y;
};
/*
Fuction Description: Image warpping
*Input: srcData-Source image buffer.
*Input: width-Source image width.
*Input: height-Source image height.
*Input: stride-Source image stride.
*Input: srcPoint-Point array before dragged.
*Input: dragPoint-Point array after dragged.
*Input: pointNum-The number of points.
*Input: gridSize-rigid size.
*Input: method-Warpping method,Similarity-1,Rigid-0.
*Output: none.
*Return: 0-OK or Failed.
*/
EXPORT int f_MLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, int srcPoint[], int dragPoint[], int pointNum, int gridSize, int method);

#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_MLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, int srcPoint[], int dragPoint[], int pointNum, int gridSize, int method);

#ifdef __cplusplus
}
#endif


#endif



#endif
