#include "string.h"
#include"stdlib.h"
#include"stdio.h"
#include"math.h"
#include"f_MLSImageWarpping.h"
#include <vector>
using std::vector;

/*Reference:Image Deformation Using Moving Least Squares
*/

static void setSrcPoints(const vector<PointD> &qsrc, vector<PointD> &newDotL, int* nPoint) {
    *nPoint = qsrc.size();
    newDotL.clear();
    newDotL.reserve(*nPoint);
    for (size_t i = 0; i < qsrc.size(); i++) 
        newDotL.push_back(qsrc[i]);
}

static void setDstPoints(const vector<PointD> &qdst,vector<PointD> &oldDotL, int* nPoint) {
    *nPoint = qdst.size();
    oldDotL.clear();
    oldDotL.reserve(*nPoint);

    for (size_t i = 0; i < qdst.size(); i++) oldDotL.push_back(qdst[i]);
}
static double bilinear_interp(double x, double y, double v11, double v12,
                              double v21, double v22) {
    return (v11 * (1 - y) + v12 * y) * (1 - x) + (v21 * (1 - y) + v22 * y) * x;
}

static double calcArea(const vector<PointD> &V) {
    PointD lt, rb;
    lt.x = lt.y = 1e10;
    rb.x = rb.y = -1e10;
    for (vector<PointD >::const_iterator i = V.begin(); i != V.end();
         i++) {
        if (i->x < lt.x) lt.x = i->x;
        if (i->x > rb.x) rb.x = i->x;
        if (i->y < lt.y) lt.y = i->y;
        if (i->y > rb.y) rb.y = i->y;
    }
    return (rb.x - lt.x) * (rb.y - lt.y);
}
static void calcDelta_rigid(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;
    PointD swq, qstar, newP, tmpP;
    double sw;

    double ratio;

    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];

    if (nPoint < 2) {
        //rDx.setTo(0);
        //rDy.setTo(0);
        return;
    }
    PointD swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    double miu_r;

    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                if (alpha == 1)
                    w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                (j - oldDotL[k].y) * (j - oldDotL[k].y));
                else
                    w[k] = pow((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                   (j - oldDotL[k].y) * (j - oldDotL[k].y),
                               -alpha);
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
                swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
				pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_r
                double s1 = 0, s2 = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					Qi.x = newDotL[k].x - qstar.x;
					Qi.y = newDotL[k].y - qstar.y;
					s1 += w[k] * (Qi.x*Pi.x+Qi.y*Pi.y);
                    s2 += w[k] * (Qi.x*PiJ.x+Qi.y*PiJ.y);
                }
                miu_r = sqrt(s1 * s1 + s2 * s2);
				curV.x -= pstar.x;
				curV.y -= pstar.y;

                curVJ.x = -curV.y, curVJ.y = curV.x;

                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					 Pi.x = oldDotL[k].x - pstar.x;
					  Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					 tmpP.x = (Pi.x*curV.x+Pi.y*curV.y)* newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y)* newDotL[k].y;
                    tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_r;
					tmpP.y *= w[k] / miu_r;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
				newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }

            if (preScale) {
                rDx[j * tarW + i] = newP.x * ratio - i;
                rDy[j * tarW + i] = newP.y * ratio - j;
            } else {
				rDx[j * tarW + i] = newP.x - i;
				rDy[j * tarW + i] = newP.y - j;
            }
        }
    }
    delete[] w;

    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static void calcDelta_Similarity(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;

    PointD swq, qstar, newP, tmpP;
    double sw;
	
    double ratio;

    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];

    if (nPoint < 2) {
        return;
    }

    PointD swp, pstar, curV, curVJ, Pi, PiJ;
    double miu_s;

    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                            (j - oldDotL[k].y) * (j - oldDotL[k].y));
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
				swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
                pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_s
                miu_s = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    miu_s += w[k] * (Pi.x*Pi.x+Pi.y*Pi.y);
                }

                curV.x -= pstar.x;
				curV.y -= pstar.y;
                curVJ.x = -curV.y, curVJ.y = curV.x;

                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;

					tmpP.x = (Pi.x*curV.x+Pi.y*curV.y) * newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y) * newDotL[k].y;
					tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_s;
					tmpP.y *= w[k] / miu_s;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
                newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }

            rDx[j * tarW + i] = newP.x - i;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
            rDy[j * tarW + i] = newP.y - j;
        }
    }

    delete[] w;
	    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static int GetNewImg(unsigned char* oriImg, int width, int height, int stride, unsigned char* tarImg, int tarW, int tarH, int tarStride, int gridSize, double* rDx, double* rDy, double transRatio)
{
    int i, j;
    double di, dj;
    double nx, ny;
    int nxi, nyi, nxi1, nyi1;
    double deltaX, deltaY;
    double w, h;
    int ni, nj;
	int pos, posa, posb, posc, posd;
    for (i = 0; i < tarH; i += gridSize)
        for (j = 0; j < tarW; j += gridSize) {
            ni = i + gridSize, nj = j + gridSize;
            w = h = gridSize;
            if (ni >= tarH) ni = tarH - 1, h = ni - i + 1;
            if (nj >= tarW) nj = tarW - 1, w = nj - j + 1;
            for (di = 0; di < h; di++)
                for (dj = 0; dj < w; dj++) {
                    deltaX =
                        bilinear_interp(di / h, dj / w, rDx[i * tarW + j], rDx[i * tarW + nj],
                                        rDx[ni * tarW + j], rDx[ni * tarW + nj]);
                    deltaY =
                        bilinear_interp(di / h, dj / w, rDy[i * tarW + j], rDy[i * tarW + nj],
                                        rDy[ni * tarW + j], rDy[ni * tarW + nj]);
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    if (nx > width - 1) nx = width - 1;
                    if (ny > height - 1) ny = height - 1;
                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);
					pos = (int)(i + di) * tarStride + ((int)(j + dj) << 2);
					posa = nyi * stride + (nxi << 2);
					posb = nyi * stride + (nxi1 << 2);
					posc = nyi1 * stride + (nxi << 2);
					posd = nyi1 * stride + (nxi1 << 2);
					tarImg[pos]     = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa],    oriImg[posb],   oriImg[posc],   oriImg[posd]);
					tarImg[pos + 1] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 1],oriImg[posb + 1], oriImg[posc + 1], oriImg[posd + 1]);
					tarImg[pos + 2] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 2],oriImg[posb + 2], oriImg[posc + 2], oriImg[posd + 2]);
					tarImg[pos + 3] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 3],oriImg[posb + 3], oriImg[posc + 3], oriImg[posd + 3]);
                }
        }
		return 0;
};

static void MLSImageWrapping(unsigned char* oriImg,int width, int height, int stride,const vector<PointD > &qsrc, const vector<PointD > &qdst, unsigned char* tarImg, int outW, int outH, int outStride, double transRatio, int preScale, int gridSize, int method)
{
	int srcW = width;
	int srcH = height;
	int tarW = outW;
    int tarH = outH;
    double alpha = 1;
	int nPoint;
	int len = tarH * tarW;
	vector<PointD> oldDotL, newDotL;
	double *rDx = NULL,*rDy = NULL;
    setSrcPoints(qsrc,newDotL,&nPoint);
    setDstPoints(qdst,oldDotL,&nPoint);
    rDx = (double*)malloc(sizeof(double) * len);
    rDy = (double*)malloc(sizeof(double) * len);
    memset(rDx, 0, sizeof(double) * len);
    memset(rDy, 0, sizeof(double) * len);
	if(method!=0)
		calcDelta_Similarity(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	else
	    calcDelta_rigid(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	GetNewImg(oriImg, srcW, srcH, stride, tarImg, tarW, tarH, outStride, gridSize, rDx, rDy, transRatio);
	if(rDx != NULL)
		free(rDx);
	if(rDy != NULL)
		free(rDy);
};
static int f_TMLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, unsigned char* dstData, int outW, int outH, int outStride, int srcPoint[], int dragPoint[], int pointNum, double intensity, int preScale, int gridSize, int method)
{
	int res = 0;
    vector<PointD> qDst;
    vector<PointD> qSrc;
	PointD point = {0};
	int len = 0;
	for(int i = 0; i < pointNum; i++)
	{
		len = (i << 1);
		point.x = srcPoint[len];
		point.y = srcPoint[len + 1];
		qSrc.push_back(point);
		point.x = dragPoint[len];
		point.y = dragPoint[len + 1];
		qDst.push_back(point);
	}
	MLSImageWrapping(srcData, width, height, stride, qSrc, qDst, dstData, outW, outH, outStride, intensity, preScale,gridSize, method);
	return res;
};
/*
Fuction Description: Image warpping
*Input: srcData-Source image buffer.
*Input: width-Source image width.
*Input: height-Source image height.
*Input: stride-Source image stride.
*Input: srcPoint-Point array before dragged.
*Input: dragPoint-Point array after dragged.
*Input: pointNum-The number of points.
*Input: gridSize-rigid size.
*Input: method-Warpping method,Similarity-1,Rigid-0.
*Output: none.
*Return: 0-OK or Failed.
*/
int f_MLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, int srcPoint[], int dragPoint[], int pointNum, int gridSize, int method)
{
	int ret = 0;
	unsigned char* dstData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	double intensity = 1.0;
	int preScale = 0;
	ret = f_TMLSImagewarpping(srcData, width, height, stride, dstData, width, height, stride, srcPoint, dragPoint, pointNum, intensity, preScale, gridSize, method);
	memcpy(srcData, dstData, sizeof(unsigned char) * height * stride);
	return ret;
};
