#ifndef _LMATRIX_H_
#define  _LMATRIX_H_

#include"LVector.h"
#include<iostream>
using namespace std;
class LMatrix4{

public:
	float matrix[16];
	LMatrix4(const float array[16]);
	LMatrix4(float a0, float a1, float a2, float a3,//col 1
		float a4, float a5, float a6, float a7,//col2
		float a8, float a9, float a10, float a11,//col3
		float a12, float a13, float a14, float a15//col4
		);
	LVector4 operator*(const LVector4& temp)const;
};
#endif
