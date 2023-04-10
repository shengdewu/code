#ifndef  __COMMON__
#define  __COMMON__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string.h>

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

#endif
