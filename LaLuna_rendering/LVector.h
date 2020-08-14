#ifndef _LVECTOR_H_
#define _LVECTOR_H_
#include<iostream>
#include<vector>
using namespace std;
class LVector4{
public:
	LVector4(float a0, float b0, float c0, float d0);

public:
	float a;
	float b;
	float c;
	float d;

	void setr(float v){ a = v; }
	void setg(float v){ b = v; }
	void setb(float v){ c = v; }
	void setalpha(float v){ d = v; }

};
#endif