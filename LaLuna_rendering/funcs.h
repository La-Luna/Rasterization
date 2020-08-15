#ifndef  _FUNCS_H_
#define _FUNCS_H_
#include "LVert.h"
#include "LVector.h"


vector<int> sortfromHigh2Low(float a, float b, float c);
LVert calculateMiddle2inTri(LVert high_v, LVert low_v, float x0);
double lineInsertWithHorizontalLine(const LVector4& top, const LVector4& bottom, float y0);
#endif
