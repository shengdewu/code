
#include"TRGB2GRAY.h"
#include"Commen.h"
#include<stdlib.h>
#include<math.h>

int f_Gray(unsigned char *srcData, int width, int height, int stride, int mode)
{
	int ret = 0;
	int i, j, gray, offset;
	offset = stride - width * 3;
	unsigned char* pSrc = srcData;
	switch(mode)
	{
	case 0://mean gray method
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		gray = (pSrc[0] + pSrc[1] + pSrc[2]) / 3;
				pSrc[0] = gray;
				pSrc[1] = gray;
				pSrc[2] = gray;
				pSrc += 3;
	    	}
			pSrc += offset;
	    }
		break;
	case 1://classic gray method
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
	    		gray = (299 *  pSrc[2] + 587 * pSrc[1] + 114 * pSrc[0]) / 100;
				pSrc[0] = gray;
				pSrc[1] = gray;
				pSrc[2] = gray;
				pSrc += 3;
	    	}
			pSrc += offset;
	    }
		break;
	case 2://photoshop gray method
		for(j = 0; j < height; j++)
	    {
	    	for(i = 0; i < width; i++)
	    	{
				gray = (MAX2(pSrc[0], MAX2(pSrc[1], pSrc[2])) + MIN2(pSrc[0], MIN2(pSrc[1], pSrc[2]))) / 2;
	    		pSrc[0] = gray;
				pSrc[1] = gray;
				pSrc[2] = gray;
				pSrc += 3;
	    	}
			pSrc += offset;
	    }
		break;
	default:
		break;
	}
	return ret;
};