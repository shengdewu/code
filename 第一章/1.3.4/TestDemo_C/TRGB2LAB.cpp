
#include"TRGB2LAB.h"
#include"Commen.h"
#include<stdlib.h>
#include<math.h>

const float T0 = 0.008856f;//216.0f / 24389.0f;
const float T1 = 7.78704f;//841.0f / (3.0f * 36.0f);
const float T2 = 0.13793f;//4.0f / 29.0f;
const float T3 = 0.20690f;//6.0f / 29.0f;
const float T4 = 0.12842f;//3.0f * 36.0f / 841.0f;
const float T = 0.00392f;//1.0f / 255.0f;
const float T5 = 1.05213f;//1.0f / 0.950456f;
const float T6 = 0.91848f;//1.0f / 1.088754f;
const float T7 = 0.39216f;//100.0f / 255.0f;
const float T8 = 0.00862f;//1.0f / 116.0f;
const float T9 = 0.002f;//1.0f / 500.0f;
const float T10 = 0.005f;//1.0f / 200.0f;
inline float FT(float t)
{
	if(t > T0)
		return pow(t,1.0f / 3.0f);
	else
		return T1 * t + T2;
}
inline float SFT(float t)
{
	if(t > T3)
		return pow(t, 3);
	else
		return T4 * (t - T2);
}
void RGB2LAB(unsigned char R, unsigned char G, unsigned char B, int *L, int* a, int*b)
{
	float X, Y, Z;
	X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
	Y = 0.212761f * R + 0.715160f * G + 0.072169f * B;
	Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;	
	X = X * T;
	Y = Y * T;
	Z = Z * T;
	*L = CLIP3(116.0f * FT(Y) - 16.0f, 0, 100);
	*a = CLIP3(500.0f * (FT(X * T5) - FT(Y)),-128,127);
	*b = CLIP3(200.0f * (FT(Y) - FT(Z * T6)),-128,127);
	*L = *L * 2.55f;
	*a = *a + 128;
	*b = *b + 128;
};
void LAB2RGB(int L, int a, int b, unsigned char * R, unsigned char * G, unsigned char *B)
{
	float X, Y, Z;
	L = L * T7;
	a = a - 128;
	b = b - 128;
	X = SFT((L + 16.0f) * T8);
	Y = 0.950456f * SFT((L + 16.0f) * T8 + a * T9);
	Z = 1.088754f * SFT((L + 16.0f) * T8 - b * T10);
	*R = CLIP3(3.240479f * X - 1.537150f * Y - 0.498535f * Z , 0, 255);
	*G = CLIP3(-0.969256f * X + 1.875992f * Y + 0.041556f * Z, 0, 255);
	*B = CLIP3(0.055648F * X - 0.204043f * Y + 1.057311f * Z , 0, 255);
}; 