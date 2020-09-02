#ifndef _MYDEFINE_H_
#define _MYDEFINE_H_
#include <iostream>
#include <limits>
const float g_windowswidth = 800;
const float g_windowheight = 800;
const float M_INF = 0.5*(numeric_limits<float>::max)();
const float M_INF_int = 0.5*(numeric_limits<int>::max)();

enum LunaProjectionMode{
	lunaOrthogonalpromode = 0,
	lunaPerspectivepromode=1
};

LVector2 curmousePos(-M_INF, -M_INF);
LVector2 lastmousPos(-M_INF, -M_INF);

bool pushleftbutton = false;
#endif // !_MYDEFINE_H_
