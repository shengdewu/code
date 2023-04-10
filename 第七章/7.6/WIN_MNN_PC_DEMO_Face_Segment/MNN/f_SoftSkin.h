
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Soft skin .
*************************************************************************/
#ifndef __T_SOFT_SKIN__
#define __T_SOFT_SKIN__


/*************************************************************************
*Function: SOFT SKIN 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*skinData: skin area buffer
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin(unsigned char* srcData, int width, int height, int stride, unsigned char* skinData, int ratio);

#endif