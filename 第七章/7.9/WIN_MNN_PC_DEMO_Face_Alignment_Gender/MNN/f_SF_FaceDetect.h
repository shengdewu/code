
#ifndef __FACE_DETECT__
#define __FACE_DETECT__
/************************************************************
*Function:  f_FaceDetect
*Description: Face detection
*Params:
*maskData:  binary image data of source image, one channel
*mWidth  : mask image width
*mHeight : mask image height
*mStride: mask image stride
*FaceRectInfos: Face Rectangle array, [faceNum, x0, y0, w0, h0, x1, y1, w1, h1, x2, y2, w2, h2, x3, y3, w3, h3], faceNum max = 4.
*Return :0-OK,or failed
************************************************************/
int f_FaceDetect(unsigned char* maskData, int mWidth, int mHeight, int mStride, int FaceRectInfos[1 + 4 * 4]);


#endif // __MK_NEAT_SKIN__







