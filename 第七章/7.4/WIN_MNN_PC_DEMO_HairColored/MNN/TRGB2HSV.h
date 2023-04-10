/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2HSV
*************************************************************************/
#ifndef  __RGB2HSI__
#define  __RGB2HSI__

void RGB2HSV(unsigned char R, unsigned char G, unsigned char B, float *H, float *S, float *V);
void HSV2RGB(float H, float S, float V, unsigned char * R, unsigned char * G, unsigned char *B);
#endif