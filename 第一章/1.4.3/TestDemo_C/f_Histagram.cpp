#include"f_Histagram.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include"Commen.h"
int f_Histagram(unsigned char *srcData, int width, int height, int stride, int hist[256], int mode)
{
	int ret = 0;
	int i, j, gray, offset;
	offset = stride - width * 4;
	unsigned char* pSrc = srcData;
	switch(mode)
	{
	case 0://Gray histagram
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		gray = (pSrc[0] + pSrc[1] + pSrc[2]) / 3;
	    		hist[gray]++;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 1://Red histagram
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		hist[pSrc[2]]++;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 2://Green histagram
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		hist[pSrc[1]]++;
	    		pSrc += 4;
	    	}
	    	pSrc += offset;
	    }
		break;
	case 3://Blue histagram
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		hist[pSrc[0]]++;
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