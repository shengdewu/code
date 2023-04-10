
#include "f_SF_ImgBase_ConnectComponentLabel.h"
#include <stdlib.h>
#include <stdio.h>
//#include "malloc.h" //on iOS platform,this header is not exist
#include "math.h"
#include "string.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a, x), b)
#define LABEL_NUM_MAX 65535

void AddLink(int links[], int a, int b)
{
    if (a < b)
    {
        int swap = a;
        a = b;
        b = swap;
    }
    if (links[a] == 0)
    {
        links[a] = b;
    }
    else if (links[a] != b)
    {
        AddLink(links, links[a], b);
        links[a] = b;
    }
};


int GetNoneZeroMin(int a, int b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    return MIN2(a, b);
}
/************************************************************
*Function:  ConnectComponentLabel
*Description: Connect Component label count
*Params:
*binaryData:  binary image data, one channel
*width  :image width
*height :image height
*labelCount: connect area label compute.
*Return :0-OK,or failed
************************************************************/
int ConnectComponentLabel(int* binaryData, int width, int height, int* labelCount)
{
	int links[LABEL_NUM_MAX];
	memset(links, 0, sizeof(int) * LABEL_NUM_MAX);
    int mark = 1;
	int pos;
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
			pos = x + y * width;
            // don't care about background pixel, background-0
            if (binaryData[pos] == 0) continue;
            // mark the point
            int upper = binaryData[pos - width];
            int left = binaryData[pos - 1];
            binaryData[pos] = GetNoneZeroMin(upper, left);
            // if upper and left neighbour have different label,
            // add or modify an entry in the link table
            if (upper != 0 && left != 0 && upper != left)
            {
                AddLink(links, upper, left);
            }
            if (binaryData[pos] == 0)
            {
                binaryData[pos] = mark++;
                if (mark == LABEL_NUM_MAX)
				{
					printf(" throw new Exception: Maximal 65535 segments supported!");
					return 1;
				}
            }
        }//x
    }//y
    // link table reconciliation
    int* compact = (int*)malloc(sizeof(int) * mark);
	memset(compact, 0, sizeof(int) * mark);
	int i;
    for (i = 1; i < mark; i++)
    {
        if (links[i] == 0) links[i] = i;
        else while (links[i] > links[links[i]]) links[i] = links[links[i]];
        compact[links[i]] = 1;
    }
    // compact groups
    int groups = 0;
    for (i = 1; i < mark; i++)
    {
        if (compact[i] > 0) compact[i] = ++groups;
    }
    // label all pixels
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
			pos = x + y * width;
            binaryData[pos]  = compact[links[binaryData[pos]]];
        }
    }
	*labelCount = groups;
	free(compact);
    return 0;
};

/************************************************************
*Function:  ConnectComponentCal
*Description: Connect Component Calculate
*Params:
*binaryData:  binary image data, one channel
*width  :image width
*height :image height
*labelAreaCount: Pixel number of each labeled area.
*labelRects: Rectangle information of each labeled area, labelRects[0], labelRects[1],...
*Return :0-OK,or failed
************************************************************/
int ConnectComponentCal(int* binaryData, int width, int height, long* labelAreaCount, TRect labelRects[])
{
	int i, j;
	int pos;
	for(j = 1; j < height - 1; j++)
	{
		for(i = 1; i < width - 1; i++)
		{
			pos = binaryData[i + j * width];
			if(pos == 0) continue;
			labelAreaCount[pos] ++;
			if(!labelRects[pos].initial)
			{
				labelRects[pos].Pos.X = i;
				labelRects[pos].Pos.Y = j;
				labelRects[pos].Left = i;
				labelRects[pos].Top = j;
				labelRects[pos].Right = i;
				labelRects[pos].Bottom = j;
				labelRects[pos].initial = true;
				continue;
			}
			labelRects[pos].Pos.X  = MIN2(labelRects[pos].Pos.X,i);
			labelRects[pos].Pos.Y  = MIN2(labelRects[pos].Pos.Y,j);
			labelRects[pos].Left   = MIN2(labelRects[pos].Left, i);
			labelRects[pos].Top    = MIN2(labelRects[pos].Top,j);
			labelRects[pos].Right  = MAX2(labelRects[pos].Right,i);
			labelRects[pos].Bottom = MAX2(labelRects[pos].Bottom,j);
		}
	}
	return 0;
};

int f_TConnectComponentLabel(int* binaryData, int width, int height, int* labelCount)
{
	int res = ConnectComponentLabel(binaryData, width, height, labelCount);
	return res;
};
int f_TConnectComponentCal(int* binaryData, int width, int height, long* labelAreaCount, TRect labelRects[])
{
	return ConnectComponentCal(binaryData, width, height, labelAreaCount, labelRects);
};
