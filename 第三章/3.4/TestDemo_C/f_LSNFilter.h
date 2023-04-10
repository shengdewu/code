
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: LSNF .
Refference: Digital image enhancement and noise filtering by use of local statistics
*************************************************************************/
#ifndef __T_LSNF__
#define __T_LSNF__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: LSNF filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*radius: filter radius,[0,++],default 10
*delta2:  delta*delta,[0,++],default 400
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_LSNFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int delta2);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_LSNFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int delta2);

#ifdef __cplusplus
}
#endif


#endif



#endif
