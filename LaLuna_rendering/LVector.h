#ifndef _LVECTOR_H_
#define _LVECTOR_H_
#include<iostream>
#include<vector>
using namespace std;
class LVector2{
public:
	LVector2(float arr[2]){
		array[0] = arr[0];		a = array[0];
		array[1] = arr[1];		b = array[1];

	}
	LVector2(float t0, float t1){
		array[0] = t0;		a = array[0];
		array[1] = t1;		b = array[1];

	}
	LVector2(){};
	LVector2(const LVector2& B){
		array[0] = B.a;
		array[1] = B.b;
	}
	~LVector2(){};
	float array[2];
	float a;
	float b;

	float getA(){ return a; }
	float getB(){ return b; }
};

class LVector4{
public:
	LVector4(){};
	LVector4(float a0, float b0, float c0, float d0);
	LVector4(const LVector4& v);
	~LVector4(){};
public:
	float a;
	float b;
	float c;
	float d;
	float array[4];
	float geta(){ return array[0]; }
	float getb(){ return array[1]; }
	float getc(){ return array[2]; }
	float getd(){ return array[3]; }
	void setr(float v){ a = v; array[0] = v; }
	void setg(float v){ b = v; array[1] = v; }
	void setb(float v){ c = v; array[2] = v; }
	void setalpha(float v){ d = v; array[3] = v; }

	float operator*(const LVector4& a);
};
#endif