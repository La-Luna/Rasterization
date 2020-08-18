#ifndef _LVERT_H_
#define _LVERT_H_

#include<vector>
#include<iostream>
#include "LVector.h"
using namespace std;

class LVert{
public:
	 
	LVector4 position;
	LVector4 color;
};


class LearlyZOutput{
public:
	float m_x, m_y, m_z;
	float m_A, m_B;
	LearlyZOutput(){};
	~LearlyZOutput(){};
};
#endif
