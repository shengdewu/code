#include"f_Test.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"TRGB2YUV.h"
#include"Commen.h"
int f_YUVAdjust(unsigned char *srcData, int width, int height, int stride, int mode)
{
	int ret = 0;
	if(srcData == NULL)
	{
		printf("input image is null!");
		return -1;
	}
	//Process
    unsigned char R, G, B;
	int y = 0, u = 0, v = 0;
	unsigned char* pSrc = srcData;
	int offset = stride - width * 4;
	switch(mode)
	{
	case 0://display Y image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2YUV(R, G, B, &y, &u, &v);
	    		pSrc[0] = y;
	    		pSrc[1] = y;
	    		pSrc[2] = y;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 1://display U image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2YUV(R, G, B, &y, &u, &v);
	    		pSrc[0] = (u + 122) * 255 / 244;
	    		pSrc[1] = (u + 122) * 255 / 244;
	    		pSrc[2] = (u + 122) * 255 / 244;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 2://display V image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2YUV(R, G, B, &y, &u, &v);
	    		pSrc[0] = (u + 157) * 255 / 314;
	    		pSrc[1] = (u + 157) * 255 / 314;
	    		pSrc[2] = (u + 157) * 255 / 314;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	default:
		break;
	}	
	return ret;
};