//
// Created by 葵ユメ on 2018/09/02.
//

#ifndef AOIYUME_MATH_UTIL
#define  AOIYUME_MATH_UTIL

#include <math.h>

#define DEGTORAD(deg)	((float)deg * (float)M_PI / 180.0f)
#define RADTODEG(rad)	((float)rad * 180.0f / (float)M_PI)
#define Lerp(b, e, r)	(b * (1.0f - r) + e * r)
#define CLAMP(v, minVal, maxVal) (std::max(std::min(v, maxVal), minVal))

#endif