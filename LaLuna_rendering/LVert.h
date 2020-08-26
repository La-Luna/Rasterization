#ifndef _LVERT_H_
#define _LVERT_H_

#include<vector>
#include<iostream>
#include "LVector.h"
using namespace std;

class LVert{
public:
	LVert(){
		texture_ID = 0;
	}
	LVector4 position;
	float origin_orth_z;
	LVector4 color;
	LVector2 texcoord;
	int texture_ID;
};


class LearlyZOutput{
public:
	float m_x, m_y, m_z;
	float m_A, m_B;
	LearlyZOutput(){};
	~LearlyZOutput(){};
};
class LFrag{

	friend LFrag;
public:
	LVector4 m_color;
	LVector4 m_position;
};

#endif
