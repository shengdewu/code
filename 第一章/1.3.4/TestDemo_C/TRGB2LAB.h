/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2LAB
*************************************************************************/
#ifndef  __RGB2LAB__
#define  __RGB2LAB__

/************************************************************
*Function:  RGB2LAB
*Description: Convert RGB to LAB
*Params:    
*R:  Red value of the pixel, range [0,255]    
*G:  Green value of the pixel, range [0,255]
*B:  Blue value of the pixel, range [0,255]
*L:  L value, range [0,255]
*a:  a value, range [0,255]
*b:  b value, range [0,255]
*Return :NULL    
************************************************************/
void RGB2LAB(unsigned char R, unsigned char G, unsigned char B, int *L, int* a, int*b);
/************************************************************
*Function:  Lab2RGB
*Description: Lab convert to RGB
*Params:    
*L:  L value, range [0,255]
*a:  a value, range [0,255]
*b:  b value, range [0,255]
*R:  Red value of the pixel, range [0,255]    
*G:  Green value of the pixel, range [0,255]
*B:  Blue value of the pixel, range [0,255]
*Return :NULL    
************************************************************/
void LAB2RGB(int L, int a, int b, unsigned char * R, unsigned char * G, unsigned char *B);
#endif