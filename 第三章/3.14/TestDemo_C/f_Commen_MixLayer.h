#ifndef __COMMEN_MIX_LAYER__
#define __COMMEN_MIX_LAYER__

int ModeLinearLight(int basePixel,int mixPixel);//���Թ�

int ModeSuperposition(int basePixel,int mixPixel);//����//��ɫ < = 128�����ɫ = ���ɫ * ��ɫ / 128����ɫ > 128�����ɫ = 255 - ��255 - ���ɫ��* (255 - ��ɫ) / 128

#endif