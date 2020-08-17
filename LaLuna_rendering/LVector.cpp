#include"stdafx.h"
#include"LVector.h"
#include<iostream>
using namespace std;
LVector4::LVector4(float a0, float b0, float c0, float d0){
	a= a0;
	b = b0;
	c = c0;
	d = d0;
	array[0] = a0;
	array[1] = b0;
	array[2] = c0;
	array[3] = d0;
}
LVector4::LVector4(const LVector4& v){
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;
	for (int i = 0; i < 4; i++){
		array[i] = v.array[i];
	}
}

float LVector4::operator*(const LVector4& a){
	float ans=0;
	for (int i = 0; i < 4; i++){
		ans += this->array[i] * a.array[i];
	}
	return ans;
}