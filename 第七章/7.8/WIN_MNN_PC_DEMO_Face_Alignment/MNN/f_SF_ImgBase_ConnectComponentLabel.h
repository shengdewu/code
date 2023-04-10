/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: Connect component label.
*************************************************************************/
#ifndef __SF_CONNECT_COMPONENT_LABEL__
#define __SF_CONNECT_COMPONENT_LABEL__

struct TPoint
{
	int X;
	int Y;
};
struct TRect
{
	bool initial;
	int isValid;
	TPoint Pos;
    int Left;
	int Top;
	int Right;
	int Bottom;
};
/************************************************************
*Function:  ConnectComponentLabel
*Description: Connect Component label count
*Params:
*binaryData:  binary image data, one channel
*width  :image width
*height :image height
*labelCount: connect area label compute.
*Return :0-OK,or failed
************************************************************/
int ConnectComponentLabel(int* binaryData, int width, int height, int* labelCount);
/************************************************************
*Function:  ConnectComponentCal
*Description: Connect Component Calculate
*Params:
*binaryData:  binary image data, one channel
*width  :image width
*height :image height
*labelAreaCount: Pixel number of each labeled area.
*labelRects: Rectangle information of each labeled area, labelRects[0], labelRects[1],...
*Return :0-OK,or failed
************************************************************/
int ConnectComponentCal(int* binaryData, int width, int height, long* labelAreaCount, TRect labelRects[]);
//
#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif //__cplusplus
EXPORT int f_TConnectComponentLabel(int* binaryData, int width, int height, int* labelCount);
EXPORT int f_TConnectComponentCal(int* binaryData, int width, int height, long* labelAreaCount, TRect labelRects[]);

#else


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int f_TConnectComponentLabel(int* binaryData, int width, int height, int* labelCount);
int f_TConnectComponentCal(int* binaryData, int width, int height, long* labelAreaCount, TRect labelRects[]);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif // _MSC_VER


#endif // __MK_NEAT_SKIN__







