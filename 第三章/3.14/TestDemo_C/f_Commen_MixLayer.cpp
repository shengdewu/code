#include"Commen.h"
#include<math.h>

int ModeLinearLight(int basePixel,int mixPixel)
{
    int res = 0;
    res = 2 * mixPixel + basePixel - 256;
    return CLIP3(res, 0, 255);
};
int ModeSuperposition(int basePixel,int mixPixel)//��ɫ < = 128�����ɫ = ���ɫ * ��ɫ / 128����ɫ > 128�����ɫ = 255 - ��255 - ���ɫ��* (255 - ��ɫ) / 128
{
    int res = 0;
    res = ((basePixel <= 128) ? (mixPixel * basePixel / 128):(255 - (255 - mixPixel) * (255 - basePixel) / 128));
    return CLIP3(res, 0, 255);
};