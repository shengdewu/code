#include"f_Test.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"TRGB2LAB.h"
#include"Commen.h"
int f_LABAdjust(unsigned char *srcData, int width, int height, int stride, int mode)
{
	int ret = 0;
	if(srcData == NULL)
	{
		printf("input image is null!");
		return -1;
	}
	//Process
    unsigned char R, G, B;
	int l = 0, a = 0, b = 0;
	unsigned char* pSrc = srcData;
	int offset = stride - width * 4;
	switch(mode)
	{
	case 0://display L image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2LAB(R, G, B, &l, &a, &b);
	    		pSrc[0] = l;
	    		pSrc[1] = l;
	    		pSrc[2] = l;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 1://display A image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2LAB(R, G, B, &l, &a, &b);
	    		pSrc[0] = a;
	    		pSrc[1] = a;
	    		pSrc[2] = a;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 2://display B image
		for(int j = 0; j < height; j++)
	    {
	    	for(int i = 0; i < width; i++)
	    	{
	    		B = pSrc[0];
	    		G = pSrc[1];
	    		R = pSrc[2];
	    		RGB2LAB(R, G, B, &l, &a, &b);
	    		pSrc[0] = b;
	    		pSrc[1] = b;
	    		pSrc[2] = b;
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